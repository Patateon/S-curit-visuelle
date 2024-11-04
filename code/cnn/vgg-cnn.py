
from sklearn import metrics
import tensorflow as tf
from tensorflow.keras import datasets, models, layers

import os


# Load dataset
# Cifar-10 (cf photomosaic)
(x_train, y_train), (x_test, y_test) = datasets.cifar10.load_data()

# Normalisation des images 
x_max = x_train.max()
x_train = x_train / x_max
x_test = x_test / x_max

"""
# Création du modèle (à tune plus tard)

Shape :
 - En entrée : (?, 28, 28, 3) 
 - En sortie : (10)

"""

model = models.Sequential()

model.add(layers.Input((32, 32, 3)))

model.add(layers.Conv2D(64, (3, 3), activation = 'relu'))
model.add(layers.Conv2D(64, (3, 3), activation = 'relu'))
model.add(layers.MaxPooling2D((2, 2)))

model.add(layers.Conv2D(128, (3, 3), activation = 'relu'))
model.add(layers.Conv2D(128, (3, 3), activation = 'relu'))
model.add(layers.MaxPooling2D((2, 2)))

model.add(layers.Conv2D(256, (3, 3), activation = 'relu'))
model.add(layers.Conv2D(256, (3, 3), activation = 'relu'))
model.add(layers.Conv2D(256, (3, 3), activation = 'relu'))
model.add(layers.Conv2D(256, (3, 3), activation = 'relu'))
model.add(layers.MaxPooling2D((2, 2)))

model.add(layers.Flatten())
model.add(layers.Dense(100, activation = 'relu'))
model.add(layers.Dense(10, activation = 'softmax'))

model.summary()

model.compile(optimizer = 'adam',
              loss = 'sparse_categorical_crossentropy',
              metrics = ['accuracy'])

batch_size = 512
epochs = 4


# Entrainement
history = model.fit(x_train, y_train,
          batch_size = batch_size,
          epochs = epochs,
          verbose = True,
          validation_data = (x_test, y_test))

# Evaluation sur le dataset de test
score = model.evaluate(x_test, y_test, verbose = 0)

print(f"Test loss : {score[0]:4.4f}")
print(f"Test accuracy : {score[1]:4.4f}")


# Sauvegarde du modèle
model_name = f"image-classifier-vgg-{epochs}-{batch_size}.keras"
model_save_location = os.path.join(os.path.dirname(__file__), "model", model_name)

model.save(model_save_location)