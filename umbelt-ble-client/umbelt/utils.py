import os

def generate_opcodes(opcode_path: str):
    """
    Reads the umbelt_opcodes.h to parse each opcode.

    Args:
        opcode_path: Path to the umbelt_opcodes.h file.

    Returns:
        A dictionary of opcode macro names to opcode values.
    """
    if not os.path.exists(opcode_path):
        raise Exception(
            f"Could not find the umbelt opcodes source path: {opcode_path}"
        )

    opcodes = {}
    with open(opcode_path, "r") as f:
        lines = f.readlines()
        for line in lines:
            if "#define" in line:

                contents = line.split()
                opcodes[contents[1]] = int(contents[2], 16)

    return opcodes

