import subprocess
import os
import shutil
import logging

# Init logger
logging.basicConfig(format="%(asctime)s %(message)s", datefmt="%Y/%m/%d %I:%M:%S%p")
LOG = logging.getLogger("Python | {file_name}".format(file_name=__name__))
LOG.setLevel(level=logging.INFO)

TMP_BLUR_DIR = os.path.join(os.path.dirname(__file__), "..", "tmp", "NAIVE_HEAVY_BLUR_[RGB]")
CNN_EXEC_FILE = os.path.join(os.path.dirname(__file__), "classifier_use.py")

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

def clear_tmp():
    to_remove = os.path.join(TMP_BLUR_DIR)
    remove_command = ["rm", "-r", "-f", to_remove]

    try:
        if os.path.exists(TMP_BLUR_DIR):
            for item in os.listdir(TMP_BLUR_DIR):
                item_path = os.path.join(TMP_BLUR_DIR, item)
                if os.path.isfile(item_path) or os.path.islink(item_path):
                    os.unlink(item_path)
                elif os.path.isdir(item_path):
                    shutil.rmtree(item_path)
            print(f"Successfully cleared contents of: {TMP_BLUR_DIR}")
        else:
            print(f"Directory does not exist: {TMP_BLUR_DIR}")
    except Exception as e:
        print(f"Error while clearing contents of {TMP_BLUR_DIR}: {e}")

def run_cnn():
    # Activate environnement
    # WIP

    # Run CNN
    classify_command = ["python", CNN_EXEC_FILE, TMP_BLUR_DIR]
    try:
        result = subprocess.run(classify_command, check=True, text=True, capture_output=True)
        print(result.stdout)
    except subprocess.CalledProcessError as e:
        print("Error while running make:")
        print(e.stderr)

if __name__ == "__main__":
    run_cnn()