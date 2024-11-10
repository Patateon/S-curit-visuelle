
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
def frac_max_pool(x):
    return tf.nn.fractional_max_pool(x, [1.0, 1.41, 1.41, 1.0], pseudo_random=True, overlapping=True)[0]

model = models.Sequential()

model.add(layers.Conv2D(filters=32, kernel_size=(3, 3), padding='same', kernel_initializer='he_uniform', input_shape=(32, 32, 3)))
model.add(layers.LeakyReLU())
model.add(layers.BatchNormalization())
model.add(layers.Conv2D(filters=32, kernel_size=(3, 3), padding='same', kernel_initializer='he_uniform'))
model.add(layers.LeakyReLU())
model.add(layers.BatchNormalization())
#model.add(Lambda(frac_max_pool))  # frac_max_pool
model.add(layers.Dropout(0.3))

model.add(layers.Conv2D(filters=64, kernel_size=(3, 3), padding='same', kernel_initializer='he_uniform'))
model.add(layers.LeakyReLU())
model.add(layers.BatchNormalization())
model.add(layers.Conv2D(filters=64, kernel_size=(3, 3), padding='same', kernel_initializer='he_uniform'))
model.add(layers.LeakyReLU())
model.add(layers.BatchNormalization())
model.add(layers.Lambda(frac_max_pool))  # frac_max_pool
model.add(layers.Dropout(0.35))

model.add(layers.Conv2D(filters=96, kernel_size=(3, 3), padding='same', kernel_initializer='he_uniform'))
model.add(layers.LeakyReLU())
model.add(layers.BatchNormalization())
model.add(layers.Conv2D(filters=96, kernel_size=(3, 3), padding='same', kernel_initializer='he_uniform'))
model.add(layers.LeakyReLU())
model.add(layers.BatchNormalization())
model.add(layers.Lambda(frac_max_pool))  # frac_max_pool
model.add(layers.Dropout(0.35))

model.add(layers.Conv2D(filters=128, kernel_size=(3, 3), padding='same', kernel_initializer='he_uniform'))
model.add(layers.LeakyReLU())
model.add(layers.BatchNormalization())
model.add(layers.Conv2D(filters=128, kernel_size=(3, 3), padding='same', kernel_initializer='he_uniform'))
model.add(layers.LeakyReLU())
model.add(layers.BatchNormalization())
model.add(layers.Lambda(frac_max_pool))  # frac_max_pool
model.add(layers.Dropout(0.4))

model.add(layers.Conv2D(filters=160, kernel_size=(3, 3), padding='same', kernel_initializer='he_uniform'))
model.add(layers.LeakyReLU())
model.add(layers.BatchNormalization())
model.add(layers.Conv2D(filters=160, kernel_size=(3, 3), padding='same', kernel_initializer='he_uniform'))
model.add(layers.LeakyReLU())
model.add(layers.BatchNormalization())
model.add(layers.Lambda(frac_max_pool))  # frac_max_pool
model.add(layers.Dropout(0.45))

model.add(layers.Conv2D(filters=192, kernel_size=(3, 3), padding='same', kernel_initializer='he_uniform'))
model.add(layers.LeakyReLU())
model.add(layers.BatchNormalization())
model.add(layers.Conv2D(filters=192, kernel_size=(3, 3), padding='same', kernel_initializer='he_uniform'))
model.add(layers.LeakyReLU())
model.add(layers.BatchNormalization())
model.add(layers.Lambda(frac_max_pool))  # frac_max_pool
model.add(layers.Dropout(0.5))

model.add(layers.Conv2D(filters=192, kernel_size=(1, 1), padding='same', kernel_initializer='he_uniform'))
model.add(layers.LeakyReLU())
model.add(layers.BatchNormalization())
model.add(layers.GlobalAveragePooling2D())
model.add(layers.Dense(units=10, kernel_initializer='he_uniform', activation='softmax'))



model.summary()

model.compile(optimizer = 'adam',
              loss = 'sparse_categorical_crossentropy',
              metrics = ['accuracy'])

batch_size = 64
epochs = 64


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