# Checkers
Checkers 1C project

Проект 1С шашки. Использование:  
- Скомпилировать main.cpp: ```bash g++ -o checkers.out main.cpp checkers.cpp```  
- Запустить checkers.out: ```bash ./checkers.out```

Команды: 
- ```move i_from j_from i_to j_to``` (Передвинуть из точки (i_from, j_from) в точку (i_to, j_to))
- ```eat i_from j_from i_to j_to``` (Съесть, перейдя из точки (i_from, j_from) в точку (i_to, j_to))
- ```end``` (Закончить партию)

