# BLDCM Electronics
These KiCad projects were made with KiCad 9.0, make sure to use the same version or a newer one.

The schematic PDFs are available in `BLDCM-Power/BLDCM-Power.pdf` and `BLDCM-MCU/BLDCM-MCU.pdf`. The Gerber files can be found in the release section of the repository.

# Install
Before opening the project, make sure to install the [`brenocq-kicad-library`](https://github.com/brenocq/brenocq-kicad-library), this library contains all symbols, footprints, and 4D models used in the projects.
- Clone the `brenocq-kicad-library` repository
- In `Preferences` -> `Configure Paths...`
  - Add a new path with the name `BRENOCQ_DIR` and the path to the cloned `brenocq-kicad-library` repository
- In `Preferences` -> `Manage Symbol Libraries...`
  - Add the `BrenoCQ.kicad_sym` symbol library from `brenocq-kicad-library`
- In `Preferences` -> `Manage Footprint Libraries...`
  - Add the `BrenoCQ.pretty/` footprint library from `brenocq-kicad-library`
