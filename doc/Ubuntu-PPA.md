Note: you can test this guide at staging.launchpad.net without affecting the
production environment, but don't forget to replace "launchpad.net" with
"staging.launchpad.net" in all links.
 
First off, you'll need a Launchpad account. You can create one at:
https://login.launchpad.net/+new_account
 
Then create a personal package archive (PPA) to host the packages at:
https://launchpad.net/people/+me/+activate-ppa
The name will default to "ppa"; choose it wisely because it won't be easy to
change it later.
 
In general you'd have to request an import of git code to Launchpad to create
daily builds of it, but cjdns code is already imported to lp:cjdns-apt
See https://help.launchpad.net/Code/Imports for information on code imports.
Launchpad only allows a project to be imported once, cjdns is already imported
by s1w so it cannot be imported by anyone else, the only way to import into
launchpad is by asking s1w to transfer ownership of his repository.
 
Finally, create a daily build recipe for the code at:
https://code.launchpad.net/+branch/cjdns-apt/+new-recipe
Default settings should do, except two things:
 - check the distribution version coverage, you probably want to support all of
   them going back to Precise
 - make sure to point the recipe to the right PPA in case you have several
 - you might want to change the versioning scheme (first line in recipe text),
   see https://help.launchpad.net/Packaging/SourceBuilds/Recipes for docs on it.
   If graceful transition from Shnatsel's PPA is desired, you're best off with
   copying it from his recipe at:
   https://code.launchpad.net/~shnatsel/+recipe/cjdns-daily
