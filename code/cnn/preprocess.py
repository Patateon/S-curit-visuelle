import subprocess

def run_makefile(target, makefile_path):
    
    command = ["make"]
    
    if makefile_path:
        command += ["-f", makefile_path]
    
    if target:
        command.append(target)
    
    try:
        result = subprocess.run(command, check=True, text=True, capture_output=True)
    except subprocess.CalledProcessError as e:
        print("Error while running make:")
        print(e.stderr)

def blur_image(images_path):
    pass

def run_cnn():
    pass

if __name__ == "__main__":
    run_makefile()