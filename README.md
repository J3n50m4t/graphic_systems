# MIDPOINT
F1 =  Δy - 0,5 Δx
F2 = F1 + Δy - Δx

```cpp
F=deltaY-0.5*deltax;
x = x0;
y = y0;
SetPixel(x,y);
for (x = x0+1, x<= x1; x++){
  if (F > 0){
    F = F+deltaY-deltaX;
    y=y+1;
  }
  else F = F +deltaY;
  SetPixel(x,y)
}
```

# Dithering

n = Farben
k = max mögliche Farben
d = gerundete schrittweite
```
d = n/k
(n/k) * (n/(k-1))

```

## Floyd Steinberg

gray < 128 -> black pixel
gray  >= 128 -> white pixel
### error Filter

# Line clipping
Gegen den Bildschirm oder gegen die Fensterarea
Bit Pattern: Oben unten rechts Links 0000


# Projektion
> Drehung gegen den Uhrzeigersinn
> ### x
> 1 |  0| 0
> --- | --- | ---
> 0 | cos a | -sin a
> 0 | sind a | cos a
>
> ###y
> cos a| 0| sina
> --- | --- | ---
> 0 | 1 | 0
> -sind a | 0 | cos a
> ###z
> cos a| -sina | 0
> --- | --- | ---
> sin a| cosa | 0
> 0 | 0 | 1

## Perspektive
> x' = xd/z

![perspektive](http://imgur.com/Kjm9yx6.jpg)

# Verdeckte Linien


```cpp
if(entered=false){
  entered = true;
  // Line just entered the face
}
else{
  entered = false;
  // Line just left the face
}
```

# Observer
