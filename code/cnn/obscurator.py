# Standard lib
import os
import time

# Interface lib
import tkinter as tk
from PIL import Image, ImageTk
from tkinter.filedialog import askopenfilename, asksaveasfilename, askdirectory

class Application(tk.Tk):

    """Basic application with tkinter"""

    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        # Title
        self.title('Obscurator')

        # Resolution
        self.geometry("640x480")
        self.resizable(0, 0)
        
        # Create a menu bar
        self.create_menu_bar()
        
        # Initialize the main frame 
        self.scene_initializer()

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
        
        self.image_frame = tk.Frame(self)
        self.canvas = tk.Canvas(self.image_frame, bg="gray", width=512, height=512)
        
        self.image_frame.pack(expand=True)
        self.canvas.pack(expand=True)
        
        main_frame.pack()
        

    def display_image(self, image_path):
        self.current_image = ImageTk.PhotoImage(Image.open(image_path))
        
        self.canvas.delete("all")
        self.canvas.create_image(20, 20, anchor=tk.NW, image=self.current_image)


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
            self.display_image(path)
            
        print(path)


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