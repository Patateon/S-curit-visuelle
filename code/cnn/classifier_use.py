import numpy as np
from tensorflow.keras.preprocessing import image
from tensorflow.keras.models import load_model

import os

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


def evaluateClass(model_path: str, image_path: str):

    model = load_model(model_path)

    img = image.load_img(image_path, target_size=(32, 32))
    img_array = image.img_to_array(img)
    img_array = np.expand_dims(img_array, axis=0)
    img_max = img_array.max()
    img_array = img_array / 255.0

    predictions = model.predict(img_array)
    predicted_class = np.argmax(predictions, axis=1)

    return predicted_class


if __name__ == "__main__":
    model_name = f"image-classifier-24-512.keras"
    model_path = os.path.join(os.path.dirname(__file__), "model", model_name)

    image_path = os.path.join("images", "chat.jpg")

    predicted_class = evaluateClass(model_path, image_path)

    print(f"Classe prédite : {predicted_class[0]} : {CIFAR_CLASS_NAME[predicted_class[0]]}")
