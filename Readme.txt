Gravity Watermelon Seed sorter system (MFC VS2019, 64bit)

Goal of the system was to sort watermelon seeds based on their ploidy (2x, 3x, and 4x).
This this case the 3x seeds were considered the rejects.

-Systems grabs data of falling seeds using 2 highspeed sentech cameras connected through a Euresys frame grabber
-Using a deep learning objection model (YOLOv5n), detect seeds based on ploidy
-Eject the 3x seeds using solenoid controlled ejectors (pressured air)