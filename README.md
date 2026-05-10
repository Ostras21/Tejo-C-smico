# 🪐 Tejo Cósmico

> Un campeón boyacense en las canchas del sistema solar.

## Idea general

**Tejo Cósmico** es un videojuego desarrollado en **C++** con **Qt** como proyecto del curso de **Programación Orientada a Objetos (POO) 2026-1**. La obra mezcla el **tejo** —deporte nacional de Colombia— con un universo *sci-fi* ambientado en el sistema solar.

El protagonista es **El Mocho**, un campeón de tejo nacido en Boyacá que, tras ser reclutado por una misteriosa federación intergaláctica, viaja por el sistema solar enfrentando rivales cósmicos en canchas imposibles. Cada planeta impone sus propias reglas físicas, sus enemigos y sus mechas por reventar.

## Niveles

| Nivel | Nombre | Tipo de vista | Objetivo |
|------|--------|--------------|----------|
| 1 | La Cancha Lunar | Vista lateral con scroll horizontal | Acertar mechas en 90 segundos esquivando viento solar y rocas oscilantes |
| 2 | El Anillo de Saturno | Vista cenital fija | Sobrevivir oleadas de enemigos espaciales que aprenden de tus patrones |

## Físicas implementadas

El juego implementa **tres** modelos físicos diferenciados:

1. **Tiro parabólico con gravedad variable** — la trayectoria del tejo cambia según el planeta o cuerpo celeste en el que se juegue.
2. **Movimiento oscilatorio senoidal** — rocas y obstáculos se desplazan siguiendo funciones seno/coseno, exigiendo lectura de patrones.
3. **Perturbación por viento solar y rebote elástico** — fuerzas externas alteran la trayectoria del tejo y los choques contra superficies aplican conservación de energía.

## Rasgos del protagonista

El Mocho cuenta con dos rasgos especiales que afectan su jugabilidad:

- 🍺 **Guarapero** — gana fuerza de lanzamiento pero pierde puntería conforme bebe.
- 🎯 **Ojo de águila** — entra en estado de concentración tras 3 mechazos seguidos, ralentizando el tiempo y mejorando la precisión.

## Agente inteligente

El **Comandante Alien** del Nivel 2 es un agente inteligente que implementa el ciclo clásico:

- **Percepción** — observa la posición, velocidad y patrones de lanzamiento del Mocho.
- **Razonamiento** — evalúa qué oleada o formación es más adecuada para contrarrestar al jugador.
- **Acción** — ordena el movimiento y los disparos de las naves enemigas.
- **Aprendizaje** — actualiza sus pesos internos partida tras partida, volviéndose más agresivo cuanto más predecible sea el jugador.

## Estructura del repositorio

```
Tejo-Cósmico/
├── MomentoI/                  # Contextualización y propuesta
├── MomentoII/                 # Diseño y diagrama de clases UML
├── MomentoIII/                # Implementación final
├── src/                       # Código fuente C++ / Qt
├── sprites/
│   ├── nivel1/                # Recursos visuales La Cancha Lunar
│   └── nivel2/                # Recursos visuales El Anillo de Saturno
├── sounds/                    # Efectos y música
├── .gitignore
└── README.md
```

## Tecnologías

- **C++17**
- **Qt 6** — `QGraphicsScene`, `QGraphicsView`, `QTimer`, `QPainter`
- **STL** — `vector`, `map`, `unique_ptr`

## Estado del proyecto

- [x] Momento I — Contextualización
- [x] Momento II — Diseño y diagrama de clases
- [ ] Momento III — Implementación y entrega final (5 de junio de 2026)
