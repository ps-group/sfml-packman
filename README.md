# Клон игры PacMan на SFML по шагам

В этих примерах описано создание клона игры шаг за шагом. Код написан на C++ в процедурном стиле для максимально лёгкого чтения. Для сборки примеров потребуется CMake, C++ компилятор (G++, Clang или MSVC) и библиотека SFML.

## Слой №1: неподвижный шарик

<div style="display:inline-block">
<img align="left" src="img/preview_1.png">
<span><a href="pacman_1">В 1-м примере</a> в программе будет заложен основной цикл игры.</span>
</div>

## Слой №2: управление персонажем

<div style="display:inline-block">
<img align="left" src="img/preview_2.png">
<span><a href="pacman_2">Во 2-м примере</a> добавлено управление персонажем с помощью клавиш-стрелок.</span>
</div>

## Слой 3: стены и проходы лабиринта

<div style="display:inline-block">
<img align="left" src="img/preview_3.png">
<span><a href="pacman_3">В 3-м примере</a> добавлены стены и проходы лабиринта, но пока ещё нет обработки столкновений</span>
</div>

## Слой 4: обработка столкновений

<div style="display:inline-block">
<img align="left" src="img/preview_4.png">
<span><a href="pacman_4">4-q пример</a> покажет, как реализовать базовую обработку столкновений (англ. collisions) со стенами лабиринта</span>
</div>

## Слой 5: призраки и поражение

<div style="display:inline-block">
<img align="left" src="img/preview_5.png">
<span><a href="pacman_5">В 5-м примере</a> появится возможность проиграть в случае, если PacMan столкнётся с призраком.</span>
</div>

## Слой 6: печенье и победа

<div style="display:inline-block">
<img align="left" src="img/preview_6.png">
<span><a href="pacman_6">В 6-м примере</a> мы добавим печенье, которое пакман должен поедать, и реализуем возможность победы.</span>
</div>

## Слой 7: улучшаем пакмана

<div style="display:inline-block">
<img align="left" src="img/preview_7.png">
<span><a href="pacman_7">В 7-м примере</a> появятся графические улучшения: мы добавим пакману ракрывающийся рот и правдоподобную анимацию движения.</span>
</div>

## Слой 8: улучшаем призраков

<div style="display:inline-block">
<img align="left" src="img/preview_8.png">
<span><a href="pacman_8">В 8-м примере</a> мы увидим врага в лицо: каждому призраку будет обеспечено собственное изображение, с анимацией, указывающей направление движения.</span>
</div>
