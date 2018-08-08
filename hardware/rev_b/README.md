Hardware Files
==============

The Berkeley Buckler was created in
[Eagle](https://www.autodesk.com/products/eagle/overview) and the `buckler.sch`
and `buckler.brd` files are Eagle schematic and board files respectively.
Schematic and board files can be viewed without Eagle in `buckler.pdf`.
`buckler_bom.xlsx` is the Bill of Materials (list of parts). It is also
provided as `txt` and `csv` files for convenience.

## Changes from prior revision
 * Change base board from the nRF52840DK to the nRF52DK (based on the nRF52832) due to availability
     * Reduced number of buttons, switches, and grove headers to deal with reduced number of pins
     * Removed connection to SD card detect signal to deal with reduced number of pins
     * Re-mapped signals to development kit pins
 * Made text bigger and added more test points
 * Shrunk board size a little


