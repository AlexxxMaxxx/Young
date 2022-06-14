import subprocess
import sys

program = "./young"
mode = 'g'

method = sys.argv[1]
dimension = sys.argv[2]
sizeDiagSt = int(sys.argv[3])
sizeDiagFn = int(sys.argv[4])
setTime = sys.argv[5]
memoryLimit = sys.argv[6] 
isLogs = sys.argv[7]

for sizeDiag in range(sizeDiagSt, sizeDiagFn + 1):
	args = [program, mode, method, dimension, str(sizeDiag), setTime, memoryLimit, isLogs]

	process = subprocess.Popen(args)
	code = process.wait()
 
if code != 0:
    print("\nError!")

