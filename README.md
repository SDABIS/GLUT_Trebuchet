# INTRODUCCIÓN

Este es un juego creado para C++, utilizando la librería gráfica GLUT. El objetivo es lanzar una piedra lo más lejos posible.
Para iniciarlo, debe ejecutarse Trebuchet.exe. Para que las texturas funcionen, las imágenes deben acompañarlo en la carpeta.

# CÓMO JUGAR

Para empezar, el jugador pulsa la barra espaciadora y comienza el movimiento. Si la pulsa de nuevo, la piedra se separará del Trebuchet y saldrá disparada. Su dirección y velocidad dependerá de la posición actual de la catapulta. También existe un medidor de velocidad: cuanto más lleno esté, más aumentará la velocidad de la piedra.

Hay 4 mapas (verano, otoño, invierno, primavera), por las que la piedra puede pasar. Cada una de ellas tiene una farola iluminándola.

El juego marcará la posición más lejana alcanzada con una línea roja. Si el jugador supera su récord, esta se actualizará.

# CONTROLES

  - Barra espaciadora: comenzar movimiento, lanzar piedra, parar medidor de velocidad, reiniciar (cuando la piedra ha parado).
  - F1, F2, F3: Modificar perspectiva.
  
# EL CÓDIGO

El código principal viene en Trebuchet.c, con todas las funciones relacionadas con el juego.
Esfera.c guarda las coordenadas necesarias para dibujar esta figura.
funciones_ejes.c contiene algunas funciones relacionadas con la cámara, y el dibujo de los ejes.
