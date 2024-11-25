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
        
        # Initialize the main frame 
        self.scene_initializer()


    def scene_initializer(self):
        """
        Load a main frame in the ugliest way possible.
        """

        main_frame = tk.Frame(self)
        
        menu_bar = tk.Menu(main_frame)
        menu_bar.config(menu=menu_bar)
        
        file_menu = tk.Menu(main_frame)
        file_menu.add_command(label="Quit", command=self.quit)
        menu_bar.add_cascade(label="File", menu=menu_bar)

        main_frame.pack()


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

        self.text_in.delete(0, 'end')
        self.text_in.insert(0, path)


    def open_dir(self):
        """Open a folder """

        path = askdirectory()

        if not path:
            return

        self.text_data.delete(0, 'end')
        self.text_data.insert(0, path)


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

        self.text_out.delete(0, 'end')
        self.text_out.insert(0, path)
     

if __name__ == "__main__":
    app = Application()
    app.mainloop()