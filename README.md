# Estructura-Dinamica-de-Datos-2026B
Este programa es un prototipo en C++ que gestiona la línea de sucesión de una familia mafiosa mediante un árbol binario. Su objetivo es automatizar el relevo de mando en tiempo real, garantizando la continuidad del liderazgo ante la muerte, encarcelamiento o retiro de sus miembros.

El sistema parsea manualmente un archivo .csv sin utilizar vectores (cumpliendo con las restricciones de memoria dinámica) y calcula la jerarquía viva mediante un recorrido en preorden.

Si el jefe actual muere, ingresa a prisión o supera los 70 años, el software busca automáticamente al sucesor idóneo en libertad; de no haber nadie disponible, rastrea en cadena a los miembros encarcelados más cercanos. Además, permite modificar datos en caliente, activando la sucesión inmediata si el jefe editado deja de cumplir con los requisitos de elegibilidad.
