def parse_keysym_file(file_path):
    """
    Parses the keysym.h file and generates a markdown table of Keysym values and their string representations.
    
    Args:
    - file_path: Path to the keysym.h file.
    
    Returns:
    - A markdown string containing the table.
    """
    keysym_table = "Keysym | String\n--- | ---\n"
    with open(file_path, 'r') as file:
        for line in file:
            if line.startswith("#define XK_"):
                parts = line.split()
                keysym = parts[1]
                string = keysym.replace("XK_", "")
                keysym_table += f"{keysym} | {string}\n"
    return keysym_table

# Specify the path to your keysym.h file
keysym_file_path = '/usr/include/X11/keysymdef.h'

# Generate the markdown table
markdown_table = parse_keysym_file(keysym_file_path)

# Print the markdown table to stdout or write it to a file
print(markdown_table)
