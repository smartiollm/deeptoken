import subprocess
import sys
import os

def compile_solution(source_path: str, output_binary: str = "main.out") -> bool:
    try:
        subprocess.run(["g++-14", "-O2", "-std=c++17", source_path, "-o", output_binary], check=True)
        return True
    except subprocess.CalledProcessError as e:
        print("Compilation failed:", e)
        return False

def run_solution(binary: str, input_file: str, output_file: str, time_limit: float = 2.0) -> int:
    try:
        with open(input_file, "r") as infile, open(output_file, "w") as outfile:
            subprocess.run(
                [f"./{binary}"],
                stdin=infile,
                stdout=outfile,
                stderr=subprocess.PIPE,
                timeout=time_limit,
                check=True
            )
        return 0
    except subprocess.TimeoutExpired:
        print(f"Time Limit Exceeded on {input_file}")
        return 1
    except subprocess.CalledProcessError as e:
        print(f"Runtime Error on {input_file}:", e.stderr.decode())
        return 2

def check_output(output_file: str, expected_file: str) -> bool:
    with open(output_file) as out, open(expected_file) as exp:
        return out.read().strip() == exp.read().strip()

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python judge.py <source.cpp> <input.txt> <expected_output.txt>")
        sys.exit(1)

    source_path = sys.argv[1]
    input_file = sys.argv[2]
    expected_file = sys.argv[3]

    output_file = "user_output.txt"
    binary = "main.out"

    if not compile_solution(source_path, binary):
        sys.exit(1)

    code = run_solution(binary, input_file, output_file)
    if code != 0:
        sys.exit(code)

    if check_output(output_file, expected_file):
        print("✅ Accepted")
    else:
        print("❌ Error")
