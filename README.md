# AirBee V1.0 en cours

L'idée est de transmettre toutes les données suivantes :
- température dans la ruche -- fait
- humidité dans la ruche -- fait
- masse de la ruche -- fait
- nombre d'abeilles entrées -- en cours
- nombre d'abeille sortie -- en cours
- tension de la batterie -- fait
Via un système 100% autonome en énergie et communication
	
## Coté alimentation : 
- Une lipo 5Ah <http://www.cdiscount.com/telephonie/batteries-telephone/easyacc-batterie-externe-5000mah-ultra-slim-charge/f-14487-eas0700686513077.html> (33€)
- Panneau solaire 3W <https://www.digikey.fr/product-detail/fr/seeed-technology-co-ltd/313070001/1597-1414-ND/5488054> (12USD)
- lipoRider <https://www.seeedstudio.com/Lipo-Rider-v1.3-p-2403.html> (9.5USD)

3 fils partent du module d'alim vers la carte (vcc,gnd,Tbat)

<img src="Picture/alim.jpg" alt="Drawing" style="width: 200px;"/>
![alt text](https://github.com/Rastafouille/AirBee/raw/master/Picture/alim.jpg )
![alt text](https://github.com/Rastafouille/AirBee/raw/master/Picture/alim3D.jpg)

## Coté intelligence :
- Carte ucontrolleur Arduino nano (5€)

## Coté communication :
- Kit Carte Breakout Sigfox BRKWS01 + Antenne +fdp TTC + un an abonnement <https://yadom.fr/carte-breakout-sfm10r1.html> (26€)

## Coté Capteur :
- Compteur I/O abeilles : projet BeeCounter2 <http://www.instructables.com/id/Honey-Bee-Counter-II/> (25USD)
- Capteur température humidité SHT11 <https://www.adafruit.com/product/246> (35USD)
- Capteur poids X4 + Load Cell Amplifier - HX711 <https://fr.aliexpress.com/item/FREE-SHIPPING-20pcs-lot-Experiments-body-scale-wholesale-human-scale-load-cell-sensor-resistance-strain-50kg/32271599811.html> et <https://fr.aliexpress.com/item/1PCS-HX711-Weighing-Sensor-Dual-Channel-24-Bit-Precision-A-D-Module-Pressure-Sensor-for-Arduino/32653087154.html?spm=a2g0s.9042311.0.0.zTDiLJ> (5€)
  
150€ en gros
vue d'ensemble au 08/10/2017
![alt text](https://github.com/Rastafouille/AirBee/raw/master/Picture/ensemble.jpg)
![alt text](https://github.com/Rastafouille/AirBee/raw/master/Picture/plateau.jpg)
![alt text](https://github.com/Rastafouille/AirBee/raw/master/Picture/electronique.jpg)

## Fonctionnement
L'Arduino centralise les données est les envoie au backend sigfox. On a droit à 140 messages de 12 Octets par jour, ca passe juste sans optimisation.

Payload : 
	
	ID::uint:8 Temp::int:16:little-endian Hum::uint:16:little-endian Poids::uint:16:little-endian IN::uint:16:little-endian OUT::uint:16:little-endian Vbat::uint:8

Sigfox renvoie les données via des CallBacks vers serveur perso (php-->json) ou backend web (Carrots et ConnectView), je suis aussi en train de regarder DjangoREST pour voir la meilleure solution.
Le but est d'avoir un dashboard pour suivre l'évolution des paramètres, sans tout redévelopper.

Il reste du travail sur l'intégration de toute l'electronique sur un plateau de ruche indépendant, sans pertuber son fonctionnement.


# Perspectives
Il va falloir travailler sur le choix des composants pour optimiser le cout total, encore trop élevé

Dessiner une carte electronique dediée.

Peut etre créer un reseau Xbee local sur 5 ruches pour un seul module Sigfox.

Rajouter une station météo?

