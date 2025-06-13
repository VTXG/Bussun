import shutil
import subprocess
import os
import sys
from pathlib import Path

def dep(path, name):
    path = Path(path)
    if not os.path.exists(path):
        err(f"{name} not found!")
    return path

def err(message: str):
    print(f"Error: {message}")
    sys.exit(1)

if len(sys.argv) < 2:
    err("Did not specify a target region.")

region = sys.argv[1]

MWCCEPPC = dep("deps/CodeWarrior/mwcceppc.exe", "CodeWarrior compiler")
MWASMEPPC = dep("deps/CodeWarrior/mwasmeppc.exe", "CodeWarrior assembler")
KAMEK = dep("deps/Kamek/Kamek.exe", "Kamek linker")
PETARI = dep([YourPetariFolder], "Petari")

rvl_sdk_path = f"{PETARI}/libs/RVL_SDK/include"
trk_path = f"{PETARI}/libs/MetroTRK/include"
runtime_path = f"{PETARI}/libs/Runtime/include"
msl_c_path = f"{PETARI}/libs/MSL_C/include"
facelib_path = f"{PETARI}/libs/RVLFaceLib/include"
jsystem_path = f"{PETARI}/libs/JSystem/include"
nw4r_path = f"{PETARI}/libs/nw4r/include"

include_cmd = f"-i include -i {PETARI}/ -I- -i {PETARI}/include -i {facelib_path} -i {rvl_sdk_path} -I- -i {trk_path} -I- -i {runtime_path} " \
             f"-I- -i {msl_c_path} -I- -i {jsystem_path} -I- -i {nw4r_path}"

compile_cmd = f"{MWCCEPPC} -c -Cpp_exceptions off -nodefaults -proc gekko -fp hard -lang=c++ " \
              f"-O4,s -inline on -rtti off -sdata 0 -sdata2 0 -align powerpc -func_align 4 " \
              f"-str pool -enum int -DGEKKO -DMTX_USE_PS {include_cmd} -D{region} -o"

assemble_cmd = f"{MWASMEPPC} -c -proc gekko {include_cmd} -D{region} -o"

if os.path.exists("build"):
    shutil.rmtree("build", ignore_errors=True)

tasks = list()
asm_tasks = list()

for root, dirs, files in os.walk("source"):
    for file in files:
        if root.find(".nocompile") >= 0:
            print(f"Skipped compilation: {file}")
            continue

        if file.endswith(".cpp"):
            source_path = os.path.join(root, file)
            build_path = source_path.replace("source", "build").replace(".cpp", ".o")

            os.makedirs(os.path.dirname(build_path), exist_ok=True)

            tasks.append((source_path, build_path))
        elif file.endswith(".s"):
            source_path = os.path.join(root, file)
            build_path = source_path.replace("source", "build").replace(".s", ".o")

            os.makedirs(os.path.dirname(build_path), exist_ok=True)

            asm_tasks.append((source_path, build_path))

if len(tasks) < 1:
    err("No C++ files to compile!")

for task in tasks:
    source_path, build_path = task

    print(f"Compiling {source_path}...")

    if subprocess.call(f"{compile_cmd} {build_path} {source_path}", shell=True) != 0:
        err("Compiler error.")

for a_task in asm_tasks:
    source_path, build_path = a_task

    print(f"Assembling {source_path}...")

    if subprocess.call(f"{assemble_cmd} {build_path} {source_path}", shell=True) != 0:
        err("Assembler error.")

print("Linking...")

object_files = " ".join([task[1] for task in tasks])
asm_obj_files = " ".join([a_task[1] for a_task in asm_tasks])

kamek_cmd = f"{KAMEK} {object_files} {asm_obj_files} -externals=symbols/{region}.txt -output-kamek=bin/CustomCode_{region}.bin"

if subprocess.call(kamek_cmd, shell=True) != 0:
    err("Linking failed.")

print("Done!")