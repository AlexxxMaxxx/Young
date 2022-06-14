import subprocess
import sys

program = "./young"
mode = "c1"

method = sys.argv[1]
dimension = sys.argv[2]
sizeDiagSt = int(sys.argv[3])
sizeDiagFn = int(sys.argv[4])

for sizeDiag in range(sizeDiagSt, sizeDiagFn + 1):
	args = [program, mode, method, dimension, str(sizeDiag)]

	process = subprocess.Popen(args)
	code = process.wait()
 
if code != 0:
    print("\nError!")

