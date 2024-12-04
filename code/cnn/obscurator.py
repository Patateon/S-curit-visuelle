# Standard lib
from email.mime import base
import os
from re import sub
import subprocess
import time
import logging
import shutil
import numpy as np

# Interface lib
import tkinter as tk
from PIL import Image, ImageTk
from tkinter.filedialog import askopenfilename, asksaveasfilename, askdirectory

# CNN Lib
import classifier_use
import preprocess

#####################

## PATH
SCRIPT_DIR = os.path.dirname(__file__)
TMP_DIR = os.path.join(SCRIPT_DIR, "..", "tmp")
TMP_OBSCURATOR = os.path.join(TMP_DIR, "obscurator")
OBSCURATOR_PATH = preprocess.OBSCURATOR_PATH 

## Filters names
FILTERS_NAME = {
    'FREQUENCY_ATTACK': os.path.join(OBSCURATOR_PATH, "filter_frequency_attack.out"),
    'HUE_SATURATION_ATTACK': os.path.join(OBSCURATOR_PATH, "filter_hue_sat_attack.out"),
    'NAIVE_HEAVY_BLUR_RGB': os.path.join(OBSCURATOR_PATH, "filter_blur.out"),
    'NAIVE_LINE_NOISE': os.path.join(OBSCURATOR_PATH, "filter_line_noise.out"),
    'NAIVE_LPB_CUT_RGB': os.path.join(OBSCURATOR_PATH, "filter_RGB_cut.out"),
    'NAIVE_LPB_CUT_YCrCb': os.path.join(OBSCURATOR_PATH, "filter_YCrCb.out"),
    'NAIVE_WAVE_DIVIDE': os.path.join(OBSCURATOR_PATH, "filter_wave_divide.out"),
    'PERLIN_HUE_SATURATION_ATTACK': os.path.join(OBSCURATOR_PATH, "filter_perlin_hue_sat.out"),
    'NON_LINEAR_RGB_PERLIN_ATTACK': os.path.join(OBSCURATOR_PATH, "filter_perlin_rgb.out"), 
}

#####################

# Init logger
logging.basicConfig(format="%(asctime)s %(message)s", datefmt="%Y/%m/%d %I:%M:%S%p")
LOG = logging.getLogger("Python | {file_name}".format(file_name=__name__))
LOG.setLevel(level=logging.DEBUG)


def apply_filter(filter_name: str, image_input: str, image_output: str):
    """
    Use the specified filter over an image and save his output.
    
    :params:
    :param filter_name: name of an existing filter inside of "obscurator"
    :param image_input: relative or absolute location of an image
    :param image_output: relative or absolute location to save image
    """
    
    if filter_name not in FILTERS_NAME:
        LOG.debug(f"Filter is not available {filter_name}")
        return False
    
    filter_path = FILTERS_NAME[filter_name]
    
    filter_command = [filter_path, image_input, image_output]
    
    try:
        result = subprocess.run(filter_command, check=True, text=True, capture_output=True)
        LOG.debug(result.stdout)
        return True
    except subprocess.CalledProcessError as e:
        LOG.error("Error while running filter:")
        LOG.error(e.stderr)
        return False

class Application(tk.Tk):

    """Basic application with tkinter"""

    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        
        ## Initialize variables
        self.current_image = None
        self.current_image_path = None
        self.filtered_image = None
        self.filtered_image_path = None
        self.current_filter = None
          
        ## Basic window settings
        # Change Title
        self.title('Obscurator')

        # Change Resolution
        self.geometry("1050x720")
        self.resizable(0, 0)
        
        ## Initialisation routines
        # Clear tmp directory
        self.clear_tmp_obscurator()
        
        # Compile filters
        self.clean_and_recompile()
        
        ## Load UI
        # Create a menu bar
        self.create_menu_bar()
        
        # Initialize the main frame 
        self.scene_initializer()
        
    def clear_tmp_obscurator(self):
        """ 
        Clear "tmp/obscurator" directory.
        """
        try:
            os.mkdir(TMP_OBSCURATOR)
        except OSError:
            pass

        try:
            if os.path.exists(TMP_OBSCURATOR):
                for item in os.listdir(TMP_OBSCURATOR):
                    item_path = os.path.join(TMP_OBSCURATOR, item)
                    if os.path.isfile(item_path) or os.path.islink(item_path):
                        os.unlink(item_path)
                    elif os.path.isdir(item_path):
                        shutil.rmtree(item_path)
                LOG.info(f"Successfully cleared contents of: {TMP_OBSCURATOR}")
            else:
                LOG.info(f"Directory does not exist: {TMP_OBSCURATOR}")
        except Exception as e:
            LOG.info(f"Error while clearing contents of {TMP_OBSCURATOR}: {e}")
    
    def clean_and_recompile(self):
        """
        Clean and recompile filter cpp
        """
        
        makefile_path = os.path.join(preprocess.OBSCURATOR_PATH, "Makefile")
        
        # Run a make clean with "obscurator/Makefile"
        preprocess.run_makefile(target="clean", makefile_path=makefile_path)
        
        # Compile everything in apps with "obscurator/Makefile" make -j
        preprocess.run_makefile(target="-j", makefile_path=makefile_path)

    def create_menu_bar(self):
        menu_bar = tk.Menu(self)
    
        menu_file = tk.Menu(menu_bar, tearoff=0)
        menu_file.add_command(label="Open...", command=self.open_file)
        menu_file.add_separator()
        menu_file.add_command(label="Exit", command=self.quit)
        menu_bar.add_cascade(label="File", menu=menu_file)
        
        self.config(menu=menu_bar)


    def scene_initializer(self):
        """
        Load a main frame in the ugliest way possible.
        """

        main_frame = tk.Frame(self)


        self.images_container_frame = tk.Frame(main_frame)
        ## Setup Image frames and controller on the left side of the window
        self.image_frame = tk.Frame(self.images_container_frame)
        
        self.image_frame_controller = tk.Frame(self.image_frame)
        
        # Add an entry and a button to the controller
        self.image_input = tk.Entry(self.image_frame_controller, width=40)
        self.image_open_button = tk.Button(self.image_frame_controller,\
            text="Open image...", command=self.open_file)
        
        # CNN Controllers
        self.input_cnn_label = tk.Label(self.image_frame_controller,\
            text="Class :")
        self.input_cnn_button = tk.Button(self.image_frame_controller,\
            text="Find class", command=self.find_input_class)
        
        self.image_input.grid(column=0, row=0)
        self.image_open_button.grid(column=1, row=0)
        
        self.input_cnn_label.grid(column=0, row=1)
        self.input_cnn_button.grid(column=1, row=1)
        
        self.image_frame_controller.pack(side="top")
        
        # Add a canvas
        self.canvas = tk.Canvas(self.image_frame, width=512, height=512, bg="blue")
    
        self.canvas.pack(side="bottom")
        
        self.image_frame.pack(side="left")
        
        ## Setup filter frame and controllers
        self.filter_frame = tk.Frame(self.images_container_frame)
        
        self.filter_controller = tk.Frame(self.filter_frame)
        
        self.filter_options = tk.StringVar(self)
        self.filter_options_names = list(FILTERS_NAME.keys())
        
        # Filter controllers
        self.scroller_filter = tk.OptionMenu(self.filter_controller,\
            self.filter_options, *self.filter_options_names,\
            command=self.set_current_filter)
        self.scroller_filter.config(width=40)
        self.apply_filter_button = tk.Button(self.filter_controller,\
            text="Apply Filter", command=self.apply_filter)
        
        # CNN Controllers
        self.output_cnn_label = tk.Label(self.filter_controller,\
            text="Class :")
        self.output_cnn_button = tk.Button(self.filter_controller,\
            text="Find class", command=self.find_output_class)
        
        self.scroller_filter.grid(column=0, row=0)
        self.apply_filter_button.grid(column=1, row=0)
        
        self.output_cnn_label.grid(column=0, row=1)
        self.output_cnn_button.grid(column=1, row=1)
        
        self.filter_controller.pack(side="top")
        
        # Filter canvas
        self.canvas_out = tk.Canvas(self.filter_frame, width=512, height=512, bg="red")
        
        self.canvas_out.pack(side="bottom")
        
        self.filter_frame.pack(side="right")
        
        # Log footer
        self.images_container_frame.pack(side="top")
        
        self.log_frame = tk.Frame(main_frame)
        
        self.log_var = tk.StringVar()
        self.log("Please open an image...")
        
        self.log_label = tk.Label(self.log_frame, textvariable=self.log_var)
        
        self.log_frame.pack(side="bottom", fill=tk.X)
        self.log_label.pack(side="bottom")
        
        main_frame.pack()
        
    def log(self, log_message: str):
        message = f"Status: {log_message}"
        LOG.info(log_message)
        self.log_var.set(message)

    def display_image(self, image_path):
        self.current_image_path = image_path
        self.current_image = ImageTk.PhotoImage(Image.open(image_path))
        
        self.canvas.delete("all")
        self.canvas.create_image(0, 0, anchor=tk.NW, image=self.current_image)

    def set_current_filter(self, selected_filter):
        self.log(f"Current filter: {selected_filter}")
        self.current_filter = selected_filter

    def apply_filter(self):
        
        if self.current_image is None\
            or self.current_image_path is None:
            self.log("There are no images")
            return
        
        if self.current_filter is None:
            self.log("Please select a filter")
            return
        
        self.clear_tmp_obscurator()
        
        self.filtered_image_path = os.path.join(TMP_OBSCURATOR, "")
        
        is_filtered = apply_filter(self.current_filter, self.current_image_path, self.filtered_image_path)
        
        basename = os.path.basename(self.current_image_path)
        basename = os.path.splitext(basename)[0] + ".png"
        self.filtered_image_path = os.path.join(self.filtered_image_path, basename)
        
        if is_filtered:
            self.filtered_image = ImageTk.PhotoImage(Image.open(self.filtered_image_path))
                    
            self.canvas_out.delete("all")
            self.canvas_out.create_image(0, 0, anchor=tk.NW, image=self.filtered_image)


    def find_input_class(self):
        
        if self.current_image_path is None:
            return
        
        current_image_class = classifier_use.evaluateClass(\
            classifier_use.VGG_LIKE, self.current_image_path)
        
        self.input_cnn_label['text'] = f"Class : {classifier_use.CIFAR_CLASS_NAME[current_image_class[0]]}"
        
    def find_output_class(self):
        
        if self.filtered_image_path is None:
            return
        
        current_image_class = classifier_use.evaluateClass(\
            classifier_use.VGG_LIKE, self.filtered_image_path)
        
        self.output_cnn_label['text'] = f"Class : {classifier_use.CIFAR_CLASS_NAME[current_image_class[0]]}"
        

    def open_file(self):
        """Open a file for editing."""

        path = askopenfilename(
            filetypes=[\
                ('Image Files', '.png .jpg .jpeg .ppm .mp4 .webm .mkv .avi .mov .flv .gif'),\
                ('All Files', '*.*')\
            ]
        )

        if not path:
            return
        
        if self.image_frame is not None:
            self.log("Please select a filter")
            self.display_image(path)
            
        self.image_input.delete(0, 'end')
        self.image_input.insert(0, path)


    def open_dir(self):
        """Open a folder """

        path = askdirectory()

        if not path:
            return


    def save_file(self):
        """Save the current file as a new file."""

        path = asksaveasfilename(
            defaultextension='.png',
            filetypes=[\
                ('Image Files', '.png .jpg .jpeg .ppm .mp4 .webm .mkv .avi .mov .flv .gif'),\
                ('All Files', '*.*')\
            ],
        )

        if not path:
            return
     

if __name__ == "__main__":
    app = Application()
    app.mainloop()