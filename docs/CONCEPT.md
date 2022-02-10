# The High Concept Document

_Based off of concept document created by Ernest W. Adams_

#### RTS Game

_Door THGA RTS-Groep 2_

## Inleiding

Een realtime strategy sidescroller, waarin de speler zijn kasteel verdedigt,
en het kasteel van de ander probeert te vernietigen. Dit kan door grondstoffen te verzamelen, en
met de verzamelde grondstoffen militaire gebouwen te bouwen. Met grondstoffen is het ook mogelijk
om een leger te bouwen.

## Features

- 3D graphics
- AI Opponent match
- 1v1 player match
- 3 aparte lanes waarop gebouwd kan worden, en waarover eenheden kunnen bewegen
- (Optional) Network multiplayer
- Controller support
- Diverse verschillende karakters, elk met een eigen doel in het spel (vernietigen/verzamelen resources, doden vijand, vernietigen vijandelijke gebouwen)
- 2D sprites & animations

## Genre

Real-time strategy

## Doelgroep

- Gamers die interesse hebben in een RTS, maar hier nog weinig ervaring mee hebben.
  De leeftijdsgrens is 7 jaar en ouder. Er zit geweld in het spel, maar het is niet realistisch en/of gedetailleerd.

## Mechanics

- 3 Gescheiden lanes
- Slimme eenheden (specifiek te richten op één taak)
- Domme eenheden (lopen vooruit en voeren hun taak uit)
- Inplannen van te bouwen gebouwen
- Grondstoffen inventaris
- Aanvals mechanics
- Strategisch inzetten van verschillende types units in de drie lanes
- Grondstoffen verzamel mechanics

## Competitie / Cooperatie

- Het spel is een strategisch duel
- Het is _niet_ mogelijk om cooperatief te spelen

## Unique Selling Points

- Het 3-banensysteem
- Een mooi 3D visueel
- 2D sprites & animations
- De zeer unieke combinatie van de 2 points hierboven
- RTS game die niet isometric is
- RTS game die heel toegankelijk is voor beginnende spelers

## Doelplatform

- Personal Computer
- Multiplatform (Getest op Windows/Linux/MacOS)

## Ontwerpdoelen

- Een 3D omgeving
- 2D invulling van de omgeving
- Het ontwerp is geslaagd als de 2D objecten naadloos in 3D over lijken te lopen
- De game moet speelbaar zijn met een framerate van minimaal 60
- Een dynamisch en strategisch spelverloop voor 2 spelers tegelijk

## Karakters, gebouwen, verhaal en setting

- _Setting_:  
  De setting is middeleeuws/fantasy

- _Verhaal_:  
  De game speelt zich af in de middeleeuwen. Er is een oorlog gaande tussen 2 kastelen; het kasteel van de mensen, en het kasteel van de kwaadaardige magische wezens.  
  De kastelen bevinden zich in een heuvelachtig gebied.  
  Er zijn 3 wegen tussen de kastelen. De koningen van de kastelen zullen strategie moeten toepassen om de vijand te verslaan en de oorlog te winnen.

- _Karakters_:

  - Omschrijving typen karakters:

    - Aanvallende eenheden:  
      Aanvallende eenheworden gebruikt om het leger van de tegenstander te verslaan. Deze eenheden hebben een hogere kracht, maar hebben geen speciale eigenschappen. Ze kunnen bijvoorbeeld niet stelen of gebouwen slopen.

    - Verdedigende eenheden:  
      Verdedigende eenheden worden een gebouw of positie op een van de paden toegewezen. Deze eenheden blijven hier dan staan.

    - Verzamelende eenheden:  
      Verzamelende eenheden kunnen naar de verschillende grondstofgebouwen gestuurd worden om daar grondstoffen te verzamelen.

    - Destructieve eenheden:  
      Destructieve eenheden kunnen gebouwen vernielen.  
      Er wordt onderscheid gemaakt tussen destructieve eenheden die extra schade doen tegen verdedigende gebouwen en destructieve eenheden die grondstofgebouwen kunnen vernietigen.

    - Stelende eenheden:
      Stelende eenheden hebben een hogere snelheid en lagere prijs dan de meeste andere eenheden in het spel.
      Ze worden gebruikt om onverdedigde grondstofgebouwen te plunderen. Stelende eenheden worden vooral in het begin van het spel gebruikt, wanneer verdedigingen nog een grote investering zijn.
      <hr>

  - Aanvallende eenheden:

    - De Voetsoldaat (Swordsman):  
      De voetsoldaat is de meest standaard eenheid in het spel. De voetsoldaat is gemiddeld in vrijwel alle opzichten (snelheid, levenspunten, prijs).

    - De Boogschutter (Archer):  
      De boogschutter heeft een boog. Deze eenheid heeft weinig levens, maar kan vanaf een afstand schieten.

    - De Barbaar (Barbarian):  
      De barbaar is een variant van de voetsoldaat. De barbaar heeft een hogere snelheid, hogere aanvalskracht en een lagere prijs.
      Als afweging heeft de barbaar minder levenspunten en is deze eenheid dus kwetsbaar voor boogschutter en verdedigingen.

    - De Ridder (Knight):  
      De ridder is een dure, maar sterke aanvalseenheid met een hoge snelheid.

  - Verdedigende eenheden:

    - De kruisboogschutter (Crossbowman):  
      De kruisboogschutter kan vanuit de boogschutterstoren schieten op eenheden van de tegenstander die te dicht in de buurt komen.

    - De Halberdier (Halberdier):  
      De halberdier is een eenheid met hoge levenspunten maar lage aanvalskracht. De halberdier doet extra schade tegen de ridder.

  - Verzamelende eenheden:

    - De Werker (Worker):  
      De werker kan op basis van welk gebouw hij heen gestuurd wordt een andere functie krijgen.
      Zo neemt hij bijvoorbeeld de rol als houthakker als hij naar de houthakkers hut gestuurd wordt.  
      Ook kan de werker gebouwen bouwen en repareren.

  - Destructieve eenheden:

    - Viking (Viking):  
      De viking is de standaard destructieve eenheid. Deze eenheid heeft een verminderde aanvalskracht maar doet extra schade tegen gebouwen.

    - Ram(Ram):  
      De Ram is een gespecializeerde eenheid die alleen schade kan aanrichten tegen gebouwen.

    - Kanon (Cannon):  
      Het Kanon is een destructieve eenheid die op afstand kan schieten op gebouwen of eenheden.  
      Het kanon schiet en beweegt erg traag, maar doet grote schade tegen zowel gebouwen als eenheden.

  - Stelende eenheden:
    - Dief (Thief):  
      Goedkope eenheid die weinig aanvalskracht heeft maar wel snel is en veel weet te stelen

    - Plunderaar (Raider):  
      Een duurdere eenheid die hogere kracht heeft (lager dan een aanvalseenheid), maar een lagere snelheid (hoger dan die van een aanvalseenheid)

- _Gebouwen_:  
  Militaire gebouwen: Schutterskamp voor boogschutters(Archery Range), stallen voor ridders(Stables), trainingskamp voor voetsoldaten(Training Camp), werkplaats voor kanonnen en rammen(Workshop), TODO: een gebouw waar destructieve en stelende eenheden gemaakt kunnen worden.
  Verdedigende gebouwen: Kasteel, Boogschutters toren, Muren, Mortier(Fougasse)?  
  Grondstoffen gebouwen: Houthakkers hut, Steenmijn, Goudmijn, Boerderij  
  Overige gebouwen: Lab/Universiteit/Smidse om de eenheden te upgraden en andere bonussen te onderzoeken.
