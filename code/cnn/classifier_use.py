from tensorflow.keras.preprocessing import image
from tensorflow.keras.models import load_model
import tensorflow as tf

import sys

import numpy as np
import glob 

import os

VGG_LIKE_NAME = f"image-classifier-vgg-64-64.keras"
VGG_LIKE = os.path.join(os.path.dirname(__file__), "model", VGG_LIKE_NAME)
    
CIFAR_CLASS_NAME = {
    0: "airplane",
    1: "automobile",
    2: "bird",
    3: "cat",
    4: "deer",
    5: "dog",
    6: "frog",
    7: "horse",
    8: "ship",
    9: "truck"
}

def frac_max_pool(x):
    return tf.nn.fractional_max_pool(x, [1.0, 1.41, 1.41, 1.0], pseudo_random=True, overlapping=True)[0]


def evaluateClass(model_path: str, image_path: str):

    model = load_model(model_path, custom_objects={'frac_max_pool': frac_max_pool})

    img = image.load_img(image_path, target_size=(32, 32), interpolation="bilinear")
    img_array = image.img_to_array(img)
    img_array = np.expand_dims(img_array, axis=0)
    img_array = img_array / 255.0

    predictions = model.predict(img_array)
    predicted_class = np.argmax(predictions, axis=1)

    return predicted_class

def evaluateClassFromModel(model, image_path: str):
    img = image.load_img(image_path, target_size=(32, 32))
    img_array = image.img_to_array(img)
    img_array = np.expand_dims(img_array, axis=0)
    img_array = img_array / 255.0

    predictions = model.predict(img_array)
    predicted_class = np.argmax(predictions, axis=1)

    return predicted_class

def main():
    assert len(sys.argv) == 2
    assert os.path.isdir(sys.argv[1])
    
    

    custom_objects = {'frac_max_pool': frac_max_pool}
    model = load_model(VGG_LIKE, custom_objects)

    images_directory_path = sys.argv[1]
    images_path_pattern = os.path.join(glob.escape(images_directory_path), "**")

    images_path_expected = glob.glob(pathname=images_path_pattern, recursive=True)

    images_path = []
    for image_path in images_path_expected:
        if os.path.isfile(image_path):
            images_path.append(image_path)

    images_path = sorted(images_path)

    tp = 0
    numberOfImages = len(images_path)

    assert numberOfImages == 100

    for i in range(numberOfImages):
        current_class = i//10
        predicted_class = evaluateClassFromModel(model, images_path[i])
        if predicted_class[0] == current_class:
            tp += 1
        print(f"Image : {images_path[i]} | "
              + f"Classe prédite : {predicted_class[0]} : {CIFAR_CLASS_NAME[predicted_class[0]]} | "
              + f"Classe attendu : {current_class} : {CIFAR_CLASS_NAME[current_class]}")

    accuracy = tp / numberOfImages
    print(f"Accuracy : {accuracy}")

if __name__ == "__main__":
    main()