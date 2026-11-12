import os
import sys


def sanitize_file(filepath):
    print(f"[*] Sanitizing {filepath}...")

    try:
        # 1. Read as binary to see exactly what's in there
        with open(filepath, 'rb') as f:
            raw_data = f.read()

        # 2. Decode intelligently
        # 'utf-8-sig' automatically consumes the BOM if present
        content = raw_data.decode('utf-8-sig', errors='ignore')

        # 3. Clean up lines
        lines = content.splitlines()
        cleaned_lines = [line.strip() for line in lines if line.strip()]

        # 4. Write back as standard UTF-8 (No BOM)
        new_path = filepath  # Overwrite original
        # Or use: new_path = filepath + ".clean" to be safe

        with open(new_path, 'w', encoding='utf-8', newline='\n') as f:
            f.write('\n'.join(cleaned_lines) + '\n')

        print(f"[+] Success! Cleaned {len(cleaned_lines)} lines.")
        print(f"[+] Saved to: {new_path}")

    except Exception as e:
        print(f"[!] Error: {e}")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python sanitize.py <file>")
    else:
        sanitize_file(sys.argv[1])