def exec_full(filepath):
    import os
    global_namespace = {
        "__file__": filepath,
        "__name__": "__main__",
    }
    with open(filepath, 'rb') as file:
        exec(compile(file.read(), filepath, 'exec'), global_namespace)

# execute the file
if(os.listdir('SimulationResults/Top1') != []):
	exec_full("GenerateTopology1Data.py")
if(os.listdir('SimulationResults/Top2') != []):
	exec_full("GenerateTopology2Data.py")
if(os.listdir('SimulationResults/Top3') != []):
	exec_full("GenerateTopology3Data.py")
if(os.listdir('SimulationResults/Top4') != []):
	exec_full("GenerateTopology4Data.py")
if(os.listdir('SimulationResults/Top1') != []):
	exec_full("GenerateTopology1Figures.py")
if(os.listdir('SimulationResults/Top2') != []):
	exec_full("GenerateTopology2Figures.py")
if(os.listdir('SimulationResults/Top3') != []):
	exec_full("GenerateTopology3Figures.py")
if(os.listdir('SimulationResults/Top4') != []):
	exec_full("GenerateTopology4Figures.py")