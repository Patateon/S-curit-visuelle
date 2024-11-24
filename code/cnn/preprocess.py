import subprocess
import os
import shutil
import logging
import sys
import glob

# Init logger
logging.basicConfig(format="%(asctime)s %(message)s", datefmt="%Y/%m/%d %I:%M:%S%p")
LOG = logging.getLogger("Python | {file_name}".format(file_name=__name__))
LOG.setLevel(level=logging.DEBUG)

# Path hell
SCRIPT_DIR = os.path.dirname(__file__)
TMP_BLUR_DIR = os.path.join(SCRIPT_DIR, "..", "tmp", "NAIVE_HEAVY_BLUR_[RGB]")
OBSCURATOR_PATH = os.path.join(SCRIPT_DIR, "..", "obscurator")
CNN_EXEC_FILE = os.path.join(SCRIPT_DIR, "classifier_use.py")

def run_makefile(target: str, makefile_path: str):
    """
    Small routine to execute a makefile.
    
    :param target: Makefile's target
    :param makefile_path: Makefile's directory
    :param program: Path to the program executable (optional)
    :param image_dir: Path to the image directory (optional)
    """
    
    command = ["make"]
    
    if makefile_path:
        command += ["-C", os.path.dirname(makefile_path)]
        # command += ["-f", makefile_path]
    
    if target:
        target_list = target.split(" ")
        command += (target_list)
        
    LOG.info(command)
    
    try:
        result = subprocess.run(command, check=True, text=True, capture_output=True)
        LOG.info(result.stdout)
    except subprocess.CalledProcessError as e:
        LOG.error("Error while running make:")
        LOG.error(e.stderr)

def blur_image(images_path):
    """
    Function to blur all images in a folder using obscurator.

    Note: Blurred images will be saved in the tmp directory

    :param images_path: Path to images directory to blur
    """

    makefile_path = os.path.join(OBSCURATOR_PATH, "Makefile")
    blur_out_path = os.path.join(os.path.dirname(makefile_path), "blur.out")
    
    makefile_path = os.path.abspath(makefile_path)
    blur_out_path = os.path.abspath(blur_out_path)
    images_path = os.path.abspath(images_path)

    # First recompile blur.cpp
    run_makefile(target="-j", makefile_path=makefile_path)

    # Execute blur.cpp upon every images in images_path
    makefile_target = f"-j all_images PROGRAM={blur_out_path} IMAGE_DIR={images_path}"
    run_makefile(target=makefile_target, makefile_path=makefile_path)


def clear_tmp():
    """
    Clear the tmp directory.
    """
    try:
        os.mkdir(TMP_BLUR_DIR)
    except OSError:
        pass

    try:
        if os.path.exists(TMP_BLUR_DIR):
            for item in os.listdir(TMP_BLUR_DIR):
                item_path = os.path.join(TMP_BLUR_DIR, item)
                if os.path.isfile(item_path) or os.path.islink(item_path):
                    os.unlink(item_path)
                elif os.path.isdir(item_path):
                    shutil.rmtree(item_path)
            LOG.info(f"Successfully cleared contents of: {TMP_BLUR_DIR}")
        else:
            LOG.info(f"Directory does not exist: {TMP_BLUR_DIR}")
    except Exception as e:
        LOG.info(f"Error while clearing contents of {TMP_BLUR_DIR}: {e}")

def run_cnn():
    """
    Execute the current model on every images in the tmp directory.

    Note: Only work if the current python environnement respect the
    "requirements.txt" and the tmp directory contains 100 images.
    """

    # Run CNN
    classify_command = ["python", CNN_EXEC_FILE, TMP_BLUR_DIR]
    try:
        result = subprocess.run(classify_command, check=True, text=True, capture_output=True)
        LOG.info(result.stdout)
    except subprocess.CalledProcessError as e:
        LOG.info("Error while running make:")
        LOG.error(e.stderr)

def main():
    
    assert len(sys.argv) == 2
    assert os.path.isdir(sys.argv[1])
    
    images_path = glob.escape(sys.argv[1])
    images_abs_path = os.path.join(SCRIPT_DIR, images_path)
    images_abs_path = os.path.abspath(images_abs_path)

    LOG.info("Clear tmp directory...")
    clear_tmp()
    LOG.info("tmp has been cleared.")

    LOG.info("Blurring images...")
    blur_image(images_abs_path)
    LOG.info("Blur done.")

    LOG.info("CNN inferences...")
    run_cnn()
    
    clear_tmp()

if __name__ == "__main__":
    main()