Zum initialisieren des repos aus vsc

auf das grüne symbol in git hub clicken dann auf https 
-> https://github.com/ScriptSmith24/Netzwek.git und so ein link erscheine sollte

dann einfach git clone https://github.com/ScriptSmith24/Netzwek.git


#Bugs
 /server.c
Zur zeit gibt es einen bug beim starten des servers so das man nicht die 
website von bht erreichen kann. Es wird warscheinlich was mit ports zu tuhen haben, Neustart der laptops sollte es fixen, wenn nicht dann empfehle ich diese 3 commands zu machen 

ipconfig /flushdns

netsh winsock reset

netsh int ip reset

danach reset und es sollte wieder klappen