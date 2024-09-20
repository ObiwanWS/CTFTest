***Note to reviewers***

I wasn't couting the hours precisely, but I believe it took me in between 30 to 36 hours to complete the test.

Due to time constraints a lot of things weren't exactly as I would like them to be in a production enviroment. With more time I would've liked to:
- Remove Code duplication from the BigMoxiWeaponComponent and BigMoxiPawnComponent, with a bit more time I would like to refactor those to one a common parent with the comon functionality to avoid the code duplication found here.
- I didn't replicate the pitch view for simulated proxies because it looked very weird to rotate the mesh, and to do this properly would require me to setup a new mesh and a new animation logic, which I believe was beyong the scope of this test so I didn't do it.
- Team logic is inside the game state, I would've liked to make a separate component to handle that instead of adding more functionality directly into the game state.
- Create a match flow component on the game state to handle rounds, pre-game and post-game, win conditions and so on.
- Make a match definition class the would go inside the level definition asset to control different match logic from the editor and making it easier to change logic.
- I didn't want to completely eliminate the first person template from the project, so the shooting is still the same weird logic, just moved to an ability instead of a completely new shooting system.
- I don't like how the weapon is handled in the FP template and with more time I would've liked to implement a propper inventory system to handle flags and weapons.
- For this test I just enabled Unreal's static mesh movement replication on the physics cubes around the level, with more time I would've liked to make my own physics replication component.
- Add validation functions to protect the RPCs from malicious client manipulations.
- Implement a propper spawn point picker depending on game state for respawns, for example; picking the respawn point that is closes to the enemy when he has the flag.
- Move more logic into a componentization approach instead of relying so much on inheritance.

But overall I am happy with what I was able to achieve in the time I used to make it.
(Note; I also made the mesh and materials for the flag and flag base just for fun.)