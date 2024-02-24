# call_404

Waking up, you found yourself at an apocalypse Earth threatened by aliens.

Call your allies, save the Earth.

You have only 44 minutes before doomsday.

# Manual

## Game Execution

1. Install pygame module with `pip install pygame`.
2. Execute the .py file with `python call_404.py` Make sure to locate the resources folder on the same route.

## Game Manual

### Location- Earth

1. Avoid the laser from UFOs, make calls to find your allies.
2. Move with direction keys.
3. After moving to the red telephone, make a call with 'C' key.
5. MAKE A CALL: The call is successfully connected with the probability of 77%. FELLOW +1
6. MAKE A CALL: Once used, the telephone disppears and is generated at a new location.
7. AVOID UFOS : If you are hit by the laser, you are teleported to inside the UFO.
8. AVOID UFOS : UFO laser disappears after it hits the end of screen.

### Location- UFO

1. Find the exit portal. You have to sacrifice some of your allies to return...
2. Move with direction keys.
3. After moving to a portal, press 'S' key to check if it is an exit.
4. EXIT UFO: The portals may be moving or fixed to a location.
5. EXIT UFO: Moving portals are never an exit, fixed portals may be an exit with the probability of 50%.
6. EXIT UFO: If the portal is an exit, you can sacrifice one ally to teleport yourslef to Earth. FELLOW -1, CAPTIVE +1
7. EXIT UFO: If the portal is not an exit, two of your allies are sacrificied. FELLOW -2, CAPTIVE +2

## Ending

### Earth is saved!

1. If Fellow > Captive.

### You are alive!

1. If you are at Earth at the moment of remaining time 00:00.

### Ending Scenarios

1. Fellow > Captive & You are at Earth: YOU ARE ALIVE AND SAVED THE EARTH!
2. Fellow > Captive & You are in a UFO: YOU ARE DEAD…BUT SAVED THE EARTH !
3. Fellow <= Captive & You are at Earth: YOU ARE ALIVE…BUT THE END IS HERE ANYWAY.
4. Fellow <= Captive & You are in a UFO: YOU ARE DEAD…BUT THE END IS HERE ANYWAY
5. Additionally, if the number of your ally is zero while you are in a UFO, the game ends with the foruth scenario... as you cannot return to Earth anyways.
