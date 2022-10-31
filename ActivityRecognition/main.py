import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt

#classes
class_names = ['Digging','Rotating','Idle']

#input
input100 = open("Data/Measure/Test2_Master_GyroZ.log", "r").readline().split()
print("read sensor data: " + str(len(input100)))
input100 = [float(i) for i in input100]
for i in range(0,len(input100)):
    if abs(input100[i]) > 10:
        input100[i] = input100[i]*2/input100[i]
input100 = [i/max(input100) for i in input100]
inputTrain = np.array(input100[:4000]).reshape(400,10)
inputVal = np.array(input100[4000: 4500]).reshape(50,10)

#truth digging
truth100 = open("Data/Truth/test2_rotate_timeline.txt", "r").readline().split()
print("read truth: " + str(len(truth100)))
truth100 = [int(i) for i in truth100]
truthTrain = np.array(truth100[15:415]).reshape(400,1)
truthVal = np.array(truth100[415:465]).reshape(50,1)

model = tf.keras.models.Sequential([
  tf.keras.layers.Flatten(input_shape=(10,)),
  tf.keras.layers.Dense(20, activation='relu'), 
  tf.keras.layers.Dense(20, activation='relu'),
  tf.keras.layers.Dense(8, activation='relu'),
  tf.keras.layers.Dropout(0.2),
  tf.keras.layers.Dense(2)
])


model.compile(optimizer='adam',
              loss=tf.keras.losses.(from_logits=True),
              metrics=['accuracy'])

model.fit(inputTrain, truthTrain, epochs=300, batch_size=1)   

#validation
results = tf.nn.softmax(model(inputVal))

for r in results:
    if r[0] < 0.5:
        print(1,end=" ")
    else:
        print(0,end=" ")
