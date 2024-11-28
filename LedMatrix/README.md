# LedMatrix

## Схема
![image](https://github.com/user-attachments/assets/22dd17e1-5fd4-4038-abb0-b978f6457360)

## Таблица

| leds  | pin7 | pin8 | pin11 | pin12 |
|-------|------|------|-------|-------|
| 1     | H  | L  | L  | H  |
| 2     | H  | L  | H  | L  |
| 3     | L  | H  | L  | H  |
| 4     | L  | H  | H  | L  |
| 1,2   | H  | L  | L  | L  |
| 1,3   | H  | H  | L  | H  |
| 2,4   | H  | H  | H  | L  |
| 3,4   | L  | H  | L  | L  |
| 1,4   | H/L | L/H | L/H | H/L |
| 2,3   | H/L | L/H | H/L | L/H |
| 1,2,3 | H/L | L/H | L/H | L/L |
| 1,2,4 | H/L | L/H | H/L | L/L |
| 1,3,4 | L/H | H/L | L/L | L/H |
| 2,3,4 | H/L | L/H | L/L | L/H |
| all   | H  | H  | L  | L  |
| zero  | L  | L  | L  | L  |
