import os

for root, _, files in os.walk("symbols/base"):
    for file_name in files:
        file_path = os.path.join(root, file_name)

        if not file_name.endswith(".txt"):
            continue

        symbols: dict[str, int] = {}

        with open(file_path, "r") as file:
            for line in file:
                line = line.strip()

                if not line:
                    continue

                name, addr = line.split("=", 1)
                addr_value = int(addr, 16)
                name_filter = name

                if not name.startswith("@"):
                    name_filter = name_filter.replace("@", "")

                name_filter = name_filter.replace("<", "").replace(">", "")

                if name_filter != name:
                    symbols[name_filter] = addr_value

                symbols[name] = addr_value

        symbols = dict(sorted(symbols.items(), key=lambda item: item[1]))

        out_path = os.path.join("symbols", file_name)
        with open(out_path, "w") as file:
            for name, addr in symbols.items():
                file.write(f"{name}=0x{addr:08X}\n")
