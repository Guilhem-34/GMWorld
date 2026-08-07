<div align="center">

![Bannière](https://capsule-render.vercel.app/api?type=waving&color=0:0f0c29,50:302b63,100:24243e&height=220&section=header&text=Station%20Sol%20Spatiale&fontSize=50&fontColor=ffffff&animation=fadeIn)

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white)
![Qt](https://img.shields.io/badge/Qt-6-41CD52?logo=qt&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-064F8C?logo=cmake&logoColor=white)
![Status](https://img.shields.io/badge/status-en%20d%C3%A9veloppement-yellow)

**Projet d'apprentissage : suivi de satellites en C++/Qt, du logiciel jusqu'au vrai matériel.**

</div>

## À propos

Application Qt de bureau qui suit des satellites (ISS, NOAA, Starlink...) en temps réel à partir
de données orbitales (TLE), affiche leur position sur une carte, et calcule les prochains
passages visibles depuis une position donnée. Le projet évolue ensuite vers une vraie station
sol physique : un rotateur d'antenne piloté automatiquement, puis la réception réelle de
signaux satellite.

C'est avant tout un projet d'apprentissage : remise à niveau en C++/Qt, et réapprentissage de
Git/GitHub en même temps, construit progressivement phase par phase.

## Roadmap

- [ ] **Phase 1 — Tracker logiciel** : lecture de TLE, calcul de position (SGP4), affichage sur une carte
- [ ] **Phase 2 — UI avancée** : vue radar azimut/élévation, notifications, graphiques
- [ ] **Phase 3 — Rotateur d'antenne** : 2 moteurs pas-à-pas + ESP32, piloté depuis l'appli
- [ ] **Phase 4 — Réception SDR** : réception réelle de signaux satellite (RTL-SDR)
- [ ] **Phase 5 — Mission Control** : dashboard complet, historique, visu 3D

## Stack technique

- **Langage** : C++17
- **Framework GUI** : Qt 6 (Qt Widgets)
- **Build system** : CMake
- **IDE** : Qt Creator

## Structure du repo

```
station-sol-spatiale/
├── docs/           # notes et documentation
├── src/            # code source (à venir en Phase 1)
├── .gitignore
└── README.md
```

## Avancement

Mise en place de l'environnement et structure du projet — le code démarre en Phase 1.

---

<div align="center">

Fait avec [Qt](https://www.qt.io/) et pas mal de café ☕ — par [Guilhem-34](https://github.com/Guilhem-34)

</div>