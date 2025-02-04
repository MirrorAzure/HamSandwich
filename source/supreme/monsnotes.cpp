#include "monsnotes.h"
#include "monster.h"

char monsNotes[NUM_MONSTERS][512]={
	// NULL
	"No notes!",
	// Bouapha
	"Bouapha was named after a guy in an English "
	"class I took in college. "
	"Bouapha's shirt actually depicts a pumpkin "
	"with the international \"NO\" sign (circle/slash) "
	"on it, intended to indicate his distaste for "
	"living psychotic pumpkins.  Why does Bouapha "
	"fight with big red hammers?  Because his "
	"original goal was to smash pumpkins! "
	"Most of Bouapha's sound effects (notably "
	"\"Hammer Up!\") are ripped directly from the "
	"original SPISPOPD game, and thusly are quite "
	"low quality.",
	// Bonehead
	"I think naming the skeletons Boneheads was "
	"clever. I am pleased with myself. When I "
	"made Loonyland, it was tougher, since I had "
	"several breeds of skeleton to deal with, but "
	"in the happy old days of Dr. Lunatic, life "
	"was blissful and kind. "
	"The sound of a Bonehead dying is actually "
	"somebody groaning horribly while we shake "
	"a 3.5\" floppy disk next to the microphone. "
	"The disk was supposed to sound like bones "
	"rattling.  You be the judge!",
	// scary bat
	"One of the more obnoxious foes, yes?  Lots "
	"of people don't know about the Scary Bat's "
	"most intriguing attack.  They don't merely "
	"divebomb you from long range!  If they get "
	"in close, they will attempt to smack you "
	"with their own eyeballs by bugging them out. "
	"It's quite an effect, look closely sometime. "
	"Why it hurts you, I do not know.",
	// eensy weensy
	"Brimstone and tarnation, these little squirts "
	"are obnoxious... terribly hard to hit, insanely "
	"fast moving, and even a little bit dangerous! "
	"Personally, I am a "
	"big fan of the chomp noise these guys make as "
	"they nip at your heels.  I also think it kind "
	"of looks cute.  On top of all that, it's the "
	"one time you have an easy chance to hit them! "
	"The name Eensy Weensy of course is an homage to "
	"the nursery rhyme \"The Eensy Weensy Spider\".",
	// spitter
	"I kinda like Spitters.  They're just classic "
	"adversaries.  Green spiders that spit acid. "
	"Not much more complex than that!  Easy to kill, "
	"not too dangerous, just reasonable.  They're the "
	"Boneheads of the arachnid kingdom.  I don't think "
	"I have much else to say about them really. ",
	// zombie
	"The classic Zombie. Couldn't be much "
	"easier to defeat, and they even cry out for "
	"\"brraaiinnsss\" and leap at your head. I "
	"like these guys - their stumbling gait, "
	"the way they sound, their lolling "
	"tongue. Yes, I am pleased with my zombies. One "
	"day I'll look back at my pathetic wreck of a "
	"career and say \"But boy, I made some nice "
	"zombies.\""
	"Though I tried for a while, I could not "
	"come up with a clever name for Zombies. Besides, "
	"they figure so prominently into the game's "
	"'plot' that I didn't think I should.",
	// egg sac
	"(bby) Take a lot of hits to kill.  One of several "
	"enemies to take advantage of my paint program's "
	"'blur' function in their death sequence.  These "
	"are nice to use to block off areas.  They're "
	"big enough to cover a 1-tile wide gap, and it's "
	"a bit of work to kill one.  Also of note is "
	"that enemies can walk right through Egg Sacs "
	"(had to be done that way to allow the Eensies "
	"to get out once they were born!). "
	"Something to think about in your level designs.",
	// mama spider
	"(bby)These can be awfully dangerous. A "
	"little tip which is fairly suicidal "
	"to use: stand just outside the range "
	"of their razor sharp mandibles (which do "
	"indescribable damage!), and they will continuously "
	"try to chomp on you rather than spit acid. "
	"The precision required and the risks involved "
	"make this usually a bad idea though. "
	"When these die, quite often their babies end "
	"up stuck on each other. Sad. Just pretend "
	"they're stuck in the goopy body parts left "
	"behind... or better, DON'T think that at all. ",
	// pygmy
	"This is another of the few personality-packed "
	"monsters.  They're also one of the few which "
	"remain still until you get close or shoot them. "
	"The pygmy expresses his personality in three "
	"ways - the angry \"Unga Bunga!\" when he spots "
	"you, and the yawn and spear spin moves he does "
	"when he's waiting for you to come near.  They "
	"serve almost no purpose but to look cute. "
	"Almost, I say?  Yes, almost.",
	// aquazoid
	"The first of the infamous and frightening Zoids. "
	"Why I decided to call them Zoids, I do not know. "
	"These are called aquazoids of course because "
	"aqua means water, and that's what they're "
	"supposed to live in.  You could put them in lava "
	"of course, but that's simply not realistic, now "
	"is it??  All the sounds that Zoids make are based "
	"on the same sound.  Their ouch sound is their "
	"shooting sound backwards, and their death sound "
	"is their ouch sound repeated, faded, and "
	"stretched out.  I like their sounds.",
	// matilda
	"Here's something shocking: Matilda was originally "
	"much much tougher than she is now.  Much.  I toned "
	"her down a lot due to feedback and being unable to "
	"beat her myself.  She's the toughest and most "
	"interesting boss because she was the first one I "
	"made, and I spent forever planning her out. "
	"The name, \"Matilda, Queen Of Chaos\" is the name of "
	"a friend of mine (who created the little Hamumu "
	"Theme you hear at the beginning of Dr. Lunatic).",
	//matilda's skull
	"DANGER: Do not use unless you know what you are doing!  This is Matilda's skull.",
	//matilda's brain
	"DANGER: Do not use unless you know what you are doing!  This is Matilda's brain.",
	//matilda's body
	"DANGER: Do not use unless you know what you are doing!  This is Matilda's body.",
	//matilda's claw
	"DANGER: Do not use unless you know what you are doing!  This is Matilda's left claw.",
	//matilda's claw2
	"DANGER: Do not use unless you know what you are doing!  This is Matilda's right claw.",
	//matilda's tail
	"DANGER: Do not use unless you know what you are doing!  This is Matilda's tail.",
	//ninjabread man
	"There are some people who find this the funniest "
	"idea in the world.  I personally feel I must have "
	"been having a really good brain day when I came up "
	"with it.  I had created a gingerbread man model to "
	"test out the 'bones' feature of my 3D modeling "
	"program, and I felt I should incorporate it into "
	"the game, and suddenly the phrase \"Ninjabread Man\" "
	"came into my head.  So, a headband, belt, and two- "
	"position mouth (smile and 'O') later, he was ready "
	"to animate.  I am very embarrassed by the sounds "
	"he makes.",
	//pumpkin
	"Because Dr. L was so different from "
	"SPISPOPD, and didn't even contain Pumpkins at all, "
	"I came up with the idea of the keychains, and "
	"having them lead to a secret level filled with "
	"pumpkins to bring back memories. "
	"I tried to make their ability similar to "
	"SPISPOPD, where they wiggled wildly at random "
	"and sucked your life away on contact. "
	"But I made it a little less screwy.  The "
	"sound they make on death is the sound Ice "
	"Pumpkins made in the original "
	"SPISPOPD. As "
	"Sol Hunt says \"It's just so satisfying!\".",
	//thingie
	"Another personality-filled enemy.  The Thingies "
	"actually have more separate actions than any "
	"other monster in the game.  When they are not "
	"fighting you, they can be looking around, "
	"blinking, napping, or just sitting still.  They "
	"can be fun just to watch in action from a "
	"distance.  Unlike many monsters, these will "
	"spontaneously decide to hunt you sometimes, "
	"rather than waiting for you to come near.",
	//mucho moss
	"(bby)The least intelligent adversary, since it's "
	"moss.  I think I'm the only one who finds the "
	"name of this one and the following one to "
	"be enormously clever.  I kept trying to "
	"convince others of the cleverness, but they "
	"didn't seem to agree.  Both players and "
	"monsters can freely walk on Mucho Moss, "
	"the only difference being the player finds "
	"it unpleasantly tingly.",
	// moss grande
	"(bby)I like the animation these use when they "
	"shoot - they twist their eyestalks together "
	"until their eyes clack into each other, like "
	"one of those two-balls-on-a-string toys. "
	"For some reason, the clacking causes bullets "
	"to fly in every direction. "
	"They spawn Mucho Moss at a high rate, if "
	"there are any open spots for it in the four "
	"tiles around them.  To really kickstart the "
	"growth of a moss field, Moss Grandes are "
	"key.  Not to mention being much more threatening "
	"than Mucho Moss to begin with.",
	// Magmazoid
	"The second of the Zoids. These are very "
	"unpleasant foes. Their fire is hard to avoid, "
	"tends to shove you into the lava, and does "
	"a whole lot of damage. "
	"Be careful with all monsters "
	"that shoot flames - there is a limit to the "
	"number of bullets active at once, and if you "
	"have too many flame attack monsters (especially "
	"Burners), you "
	"can actually make Bouapha's hammers almost useless. "
	"They just won't come out! "
	"Other monsters that shoot tons of bullets can "
	"cause this problem too.",
	// Shroom
	"This is the mushroom model from Eddie Galaxy, "
	"with feet stuck on it.  I think they're quite "
	"cute.  I personally don't find these the least "
	"bit threatening, as you even have time to move "
	"away if they begin to gas you, and they're "
	"exceedingly slow-moving.  As far as I can "
	"recall, Shrooms make no noise at all.",
	// Mush The Shroom
	"I like Mush in many ways.  His stupid sounding "
	"angry roar when he spots you, his huge "
	"Shroom Cannon (one of the coolest weapons in "
	"the game!  Except when the shrooms get stuck "
	"on something), his fully automatic Spore Gun. "
	"Heck, he's even got mushroom tops for feet! "
	"I have designed many games starring Mush in "
	"an all out action fest, and hopefully "
	"someday one will come to fruition.  He "
	"deserves it. "
	"His original name was \"Mush, Lord Of Shrooms\", "
	"but that was too long to fit.",
	// The Thing
	"(bby)This was originally intended to be in "
	"pitch darkness, with just the tentacles "
	"visible. It was very Lovecraft inspired, "
	"even the name - it's called "
	"The Thing because it's simply unspeakable! "
	"In the end, it was just cute, and "
	"vomits up Thingies, which are cuter still. "
	"The Thing is very Kermit-like in appearance. "
	"Have you noticed that the tentacles are "
	"vogue-ing? That was unplanned, I really "
	"wanted them to be flailing more like "
	"tentacles, but still, seeing a huge "
	"horror doing the Vogue is "
	"rather unique.",
	// thing tentacle
	"DANGER: Do not use unless you know what you are doing!  This is a tentacle segment for The Thing.",
	// thing tentacle tip
	"DANGER: Do not use unless you know what you are doing!  This is the tip of a tentacle for The Thing.",
	// Super Zombie
	"These guys are scary tough.  The chest-pound "
	"attack was based very directly on Goro from "
	"Mortal Kombat. In fact, the whole reason "
	"I gave them four arms was so I could do that. "
	"If you meet a Super Zombie in a situation "
	"where you can't pin him on a corner, he can "
	"be quite a terror!  Sadly, if you CAN pin "
	"him, he's a joke!  Not a very well balanced "
	"foe...",
	// Happy Stick Man
	"This guy was originally a secret character in "
	"a fabulous LITTLE game I made called Mucho "
	"Kombat. By little, I mean it was a fighting "
	"game, but the characters were only about 20 pixels "
	"tall. They were very cute! That game was fun "
	"and completely ridiculously unbalanced and random "
	"and buggy.  Yikes.  I've since decided HSM "
	"belongs in every game, or at least one of his "
	"kin, or something reminiscent of him.  The beauty "
	"of him is that he's so entertaining, and yet he "
	"takes all of five seconds to draw. ",
	// Baby SEAL
	"Okay, here's the idea behind these guys... "
	"see, there's this elite military force called "
	"the SEALs (stands for something, like all "
	"military terms). I put 2 and 2 together, and "
	"thought fuzzy baby seals with machine guns "
	"would be funny. Personally, I think I was "
	"right, though there have been some noises in the "
	"other direction. I love the pitifully sad "
	"noises they make when injured... you almost "
	"can't bear to hurt them - then they unload "
	"another burst of gunfire at you! ",
	// Cryozoid
	"The third type of Zoid.  When I first made the "
	"Aquazoid, I had no thought of making a Zoid "
	"type for each world, but I'm glad I did.  I "
	"like the symmetry of it. Cryo, of course, "
	"means ice. I think these are the cutest "
	"Zoids with their fuzzy ears.  They're also "
	"quite fun with their sneaky hiding ability.",
	// Snowguy
	"See, the joke here is supposed to be some "
	"sort of play on snowman.  I'm not entirely "
	"sure it's really even a joke actually.  But "
	"I sorta like the name.  This guy began life "
	"as a playable character in the game SPISHulk, "
	"which was quite cool.  He had this nifty "
	"throw he could do where he'd grab his foe, fly up "
	"into the air with him, and do a backbreaker. "
	"Making fighting games can be "
	"fun! "
	"The corn cob pipe sort of undermines his "
	"coolness factor, doesn't it?",
	// Pengulon
	"These guys are adorable! A fun "
	"thing to do is run around near a whole "
	"bunch of them. When you get close to one, "
	"it changes modes from wandering to flying "
	"in a circle around you, pecking at you "
	"occasionally. If you rack up a whole pile "
	"of them, it's fun to see. There is a "
	"slight joke involved in these guys, since "
	"penguins are flightless birds. "
	"See, I figure if I throw in enough really "
	"minor jests, it just elevates the whole "
	"mood.",
	// Zomboni
	"(bby)An obvious choice when trying to think up "
	"ice-based enemies in a game that has Zombies. "
	"I think this was a case where a word kept "
	"bouncing around my brain a while and I suddenly "
	"realized the implications. "
	"These guys are very dangerous.  But I like "
	"how the wheels fall off the zamboni when they "
	"blow up. "
	"Yes, I know zambonis don't look much like this, "
	"but it's what I felt like making.  I thought the "
	"spikes would be a nice touch, for that evil "
	"angry zombie look.",
	// Sven
	"Originally from SPISPOPD, wherein there were "
	"simply three unnamed \"Yeti Brothers\". "
	"Sven is the easier of the two brothers by "
	"about five million times. "
	"Why do yeti look like teddy bears? Because "
	"I had a model I had been working on of "
	"a teddy bear. I actually don't know what "
	"the original purpose of it was. But I "
	"changed the fur, added on arms and legs, "
	"and put on a "
	"silly hat, and it's a true yeti! "
	"When they yodel, those snowballs fall "
	"because they're triggering some "
	"sort of cosmic avalanche.",
	// Bjorn
	"Those ice spikes turned out to be far "
	"more deadly than originally intended, "
	"but without them, these boys would've "
	"made a very inadequate boss. "
	"Bjorn was originally 100% identical to Sven in "
	"appearance, but with the Fun Pack, I was able "
	"to give them different colored hats. "
	"The doofy use of the \"magic rock\" in "
	"the original boss level is just a "
	"result of wanting you to have to come up "
	"on each yeti in turn and see their face as they "
	"wave at you and say hi.",
	// Geozoid
	"Zoid #4! A little known fact about these "
	"is that they DO in fact move.  But they "
	"do so very very slowly... how fast could "
	"YOU move with most of your body under the "
	"sand? These guys are tough because of those "
	"darn rocks.  I have nothing else interesting "
	"to say about them.",
	// Mumble
	"Mumbles have a feature that is completely "
	"unique in Dr. Lunatic: they not only "
	"remain still until you come near, but "
	"they don't even care if "
	"you shoot them if you don't come close. This "
	"can make them very easy to beat! As if they "
	"weren't easy enough with their incredibly "
	"slow movement. On top of all that, they "
	"never deviate from their chase, "
	"which of course means pinning them behind a "
	"wall is trivially easy! Not that you need "
	"to - like I said, they're slow.",
	// Djinni
	"Sol Hunt cannot STAND these guys.  I always "
	"tell her \"just keep moving and they can't "
	"hit you!\", but she gets killed every time. "
	"They are kind of obnoxious.  The ability "
	"to 'round them up', making them all overlap "
	"and be in one big pile, can lead to some "
	"massive kills with cherry bombs! "
	"I personally think these are one of the "
	"ugliest enemies in the game, with those "
	"bulbous red eyes.  Yuck. ",
	// Magic Lamp
	"(bby)Can't have djinnis without these!  I "
	"am very proud of how the djinni spawning "
	"occurs, I think it looks JUST like in "
	"I Dream Of Jeannie. "
	"If you kill a Magic Lamp, all the Djinnis "
	"that came from it die instantly. "
	"What is up with that death sequence for "
	"these guys?  I don't know.  I couldn't "
	"think up a good way for them to die, "
	"so I went \"pblpbhpbhpbtlpbpbthttt\" and "
	"made them do that jiggle they do. "
	"Really dumb.",
	// Cacton
	"Obnoxious.  The wheels on the Cacton "
	"are actually the front landing gear "
	"from a spaceship model I made once. "
	"And the cactus is the cactus from "
	"Eddie Galaxy!  The magic of reusable "
	"art! "
	"To fight Cactons, you have to start "
	"sprinting after them the second they "
	"fire.  Usually you can kill them "
	"before they roll off to a new spot. "
	"If they do start rolling away, hide! "
	"Otherwise you'll have a faceful of "
	"needles before you have another "
	"chance to hit them.",
	// Roly Poly
	"When I invented the Dusty Desert, "
	"I had planned to fill it with all "
	"sorts of traps, like the Burners. "
	"The Roly Polies are the only ones "
	"that ever came to be. Most people "
	"never get to see the cool and deadly "
	"thing they do when destroyed. "
	"Actually, the only reason they CAN "
	"be killed was because I liked the "
	"name I came up with for them, but "
	"I realized that if they were "
	"indestructible, you'd never see their "
	"name.  You're not actually intended "
	"to ever kill them, except in some "
	"add-on worlds.",
	// Richie Lich
	"People think he's dangerous, but I "
	"don't. All you do is run around "
	"under him until he shoots "
	"his deathbeam (he only does "
	"so usually if you are under him), "
	"then step aside and take the opportunity "
	"to unload your hammers on him (he moves "
	"very slowly while shooting the beam). "
	"If you do it right and are lucky, he'll "
	"practically fire it continuously. "
	"Don't get too close though - he's "
	"poisonous to the touch! "
	"He's something of a superstar, having "
	"appeared in both Spooky Castles AND "
	"Dr. Lunatic.",
	// Dust Devil
	"When using a Dust Devil in your level, "
	"remember that the range of its sucking "
	"is infinite. So don't "
	"assume that you can have the player in "
	"one room and the Devil in another, and "
	"the player will not be sucked in. "
	"Because of this, they can be hard to use "
	"in a level design. "
	"It's actually always possible to move away "
	"from the Dust Devil, no matter how much "
	"suction it's applying. It's just a matter "
	"of finding the right angle to move at. ",
	// MechaBouapha
	"These are some of the toughest foes, as "
	"well they should be! When injured enough, "
	"they try to sneak away by melting into a "
	"small metal ball and flying away. If "
	"you corner it, you can keep it in a perpetual "
	"state of metal ballness, although usually "
	"it will eventually escape if you don't have "
	"a whole lot of firepower. "
	"This monster is of course based on the "
	"T1000 from Terminator 2. The antenna is "
	"just there because I felt bad using the "
	"exact some model I did for Bouapha.",
	// sphinx arm
	"DANGER: Do not use unless you know what you are doing!  This is Sphinxter's left arm.",
	// sphinx arm2
	"DANGER: Do not use unless you know what you are doing!  This is Sphinxter's right arm.",
	// Sphinxter
	"(bby)When naming this guy, I didn't think AT "
	"ALL of the dirty sound of it... rather, "
	"I was thinking of the word Sphinx, "
	"and I was thinking of making him like "
	"a little kid... Sphinxter "
	"sounded good! I can be a bit dense. "
	"Remember when putting this guy into "
	"your levels, that you must cover his "
	"sides and back with walls or the player can just stand "
	"and hammer him safely. "
	"There are several bosses like that. "
	"Always make sure you think about "
	"the situation you're putting a monster "
	"in!",
	// Freakazoid
	"The final, and most vile of the original Zoids. "
	"Nobody likes Freakazoids.  Basically, "
	"any creature that shoots those cactus "
	"spines is not your friend.  Their "
	"range is insane, and they always come "
	"in large numbers.  And they're hard "
	"to see too! Use Freakazoids sparingly.",
	// centibody
	"DANGER: Do not use unless you know what you are doing!  This is a body segment of the Centipumpkin.",
	// CentiPumpkin
	"This is kind of a fun boss.  Can be "
	"very tough if you have limited firepower, "
	"but very easy if you have a lot. "
	"Takes just the right amount to make it "
	"a good challenge.  This is a remake of "
	"a boss from the original SPISPOPD. "
	"Like all good centipedes, this one "
	"splits into multiple creatures if "
	"shot in the middle.  Sometimes "
	"splitting it can make it easier to "
	"beat.  Other times, it can just get "
	"you swamped.  Tricky!",
	// Wacko
	"The sound when he stabs at you is "
	"supposed to be like in Psycho - "
	"the whole \"REE REE REE\" thing.  Didn't "
	"really work out, but it does sound "
	"like the swish of a knife!  Sorta. "
	"Notice there are buttons on the BACK "
	"of his shirt, and small straps dangling "
	"from his wrists.  That's because he "
	"is in a straitjacket that he somehow "
	"got free from!  And obviously found a "
	"big knife too.  This is not a well run "
	"asylum.",
	// The Boiler
	"Entirely created because I was out of ideas "
	"for levels based in an asylum, and decided "
	"to make one called The Boiler Room. "
	"Like Sphinxter, this is a guy you want to "
	"have in a specific situation - it's very "
	"deadly downward at short range, but no "
	"threat at all from the sides or back (the "
	"grenades it lobs do not go behind or to "
	"the sides!).  So make sure the player "
	"can't get behind it.",
	// Great Pumpkin
	"(bby)Another boss from SPISPOPD redone. That "
	"of course was the concept behind the "
	"keychain levels after all! In SPISPOPD "
	"though, it was the first and easiest boss. "
	"In this game, it's quite a piece of work! "
	"Not really very tough to beat if you have "
	"a lot of firepower, but if you don't, those "
	"pumpkins can really swamp you! "
	"I'm not sure a lot of people realize that "
	"the Great Pumpkin also shoots red bullets "
	"from its eyes.  It can be hard to notice in "
	"all the mayhem!  And it's not very dangerous.",
	// The Ultrazoid
	"A mother for all "
	"the zoids you've seen, which fires everything "
	"they do, all at once. "
	"Like Sphinxter, this is one "
	"you can't let the player get behind - "
	"if he does, it's no threat at "
	"all. Of course, if he doesn't, YIKES. I "
	"even built the original Ultrazoid level to "
	"give you a chance to hide up on the side, "
	"simply because I didn't think it was fair "
	"otherwise. "
	"Note that the Ultrazoid and Great Pumpkin are "
	"the 'secret enemies' in the Asylum - even the secret guys are "
	"'bosses' in this final world!",
	// Dr Lunatic
	"I thought it would be amusing for the big badguy "
	"you've been chasing all along to be completely "
	"incapable of fighting.  I thought "
	"it would be nice for ONCE to have some semblance "
	"of reality, where somebody "
	"who seemed to be an evil scientist didn't turn out "
	"to be also a superhuman kickboxing champion. "
	"Of course, that would've been a letdown. So, "
	"he turns into a Super Duper Zombie! "
	"Why his pants grow enough to still fit? "
	"For propriety. Good enough for the Hulk, good "
	"enough for Dr. Lunatic.",
	// Super Duper Zombie
	"Kind of an easy final boss really.  Just run around, "
	"don't get in front of him, and whenever he smashes "
	"the ground, stand still!  That lessens your chance of "
	"being hit by a rock.  With reflect hammers, you can get "
	"him near a wall and smash him doubly.  His sound "
	"effects are deafening, crunchy, and horribly clipped. "
	"I think they work great!  He couldn't sound much "
	"scarier!  I pretty much cranked every dial in my "
	"sound editing program to the max, figuring this was "
	"the final boss and all. ",
	// Santa Claus
	"I racked my brains for a very very long time to come up "
	"with a secret enemy for the Icy Mountain.  I don't know "
	"why on earth it took so long to come up with the obvious. "
	"The voices turned out very stupid... He's supposed to "
	"sound really angry, as if he's punctuating each smack of "
	"you with an angry \"HO!\".  One thing about the voices "
	"though, I thought it would be cute if the only word he "
	"used was \"Ho\" and he does.  When he hits you, when he gets "
	"hurt... does it do it when he dies?  I don't recall!",
	// Mine Cart
	"This is by far the trickiest thing to include in a level. "
	"There are a million ways you can make it break the game. "
	"What I ended up "
	"deciding while making it was simply that it was better "
	"to include it that way then not have the fun of riding "
	"on mine carts.  I had high hopes for how these would "
	"work when conceiving the idea, but the end result is "
	"pretty weak.  Very little player control and so on. "
	"Still I think mine carts do add a lot of fun to the game. "
	"Especially when you run things over!",
	// Raft
	"Like the mine cart, this did not even come close to "
	"meeting my expectations.  I had visions of great complex "
	"puzzles where you had to pick the right rafts and pick "
	"the right direction to ride them in and so on, but in "
	"practice, there are very few different things a raft "
	"can do, and so you end up with only very basic puzzles. "
	"Sorry about that!",
	// powerbouapha
	"DANGER: Do not use unless you know what you are doing! "
	"This is the dummy monster for Bouapha in Power Armor.",
	// Vampire
	"These are outrageously tough enemies, and they are "
	"supposed to be. The idea behind them was to not "
	"have to fight them usually. Instead, you'd have to get "
	"past them, with the use of Garlic. "
	"Their attack is to literally just shove you with their "
	"hands and say \"Bluh!\". Why? Because for some stupid "
	"reason, in Heroes Of Might & Magic 2, that's what "
	"vampire troops did, and I always thought it was the "
	"most hilarious thing. Why on earth is a vampire just "
	"shoving you around like a little schoolyard bully?",
	// Coffin
	"(bby)Technically, a Coffin can be killed. But in practice, "
	"it can't. This is because it has a lot of life, and "
	"as soon as it is hit once, it begins to launch a Vampire "
	"at you. "
	"I included these to have a nifty way to put in vampires "
	"that aren't chasing you from the get-go, but become "
	"deadly if you tread in their territory. "
	"Did you notice the vampires have capes made of some "
	"sort of rigid plastic? Didn't want to model it getting "
	"all furly and whatnot.",
	// Spooky Ghost
	"Can't have a haunted mansion without these!  They "
	"are invisible until the player comes near or shoots "
	"them.  Each time they are hit, if they weren't busy "
	"screaming at you, they teleport somewhere else, "
	"making them a little more time-consuming to beat. "
	"They can also teleport at random, to spice things "
	"up! "
	"Their scream hurts you if you are within a certain "
	"range of it.  It's not very wide, just a couple tiles.",
	// Burner
	"Invincible and not really an enemy.  More like a "
	"trap. These can be quite evil. In my own level, "
	"Spontaneous Combustion in Halloween Horror-MH, I "
	"ended up having to remove many of them, because it "
	"kept ending up that 3/4 of the hammers Bouapha was "
	"throwing would simply not come out. Remember, Burners "
	"use up an enormous amount of the limited supply of "
	"bullets-at-a-time, because they all fire at the exact "
	"same time, and fire many many individual flames.",
	// Lefty
	"I'm a lefty myself. Isn't it cute how they can "
	"wave at you? I really wanted them to give you the "
	"finger when you shot them, but that's really not "
	"very family friendly. Originally, they had some sound effects, but I "
	"removed them because they really don't have mouths. "
	"The only sound they kept is the one "
	"when they die. Which still "
	"doesn't make sense. But you know, "
	"Boneheads are lacking in the lung and "
	"voicebox department, and they sure babble on.",
	// Pygmy Hunter
	"I was originally going to make a huge collection of "
	"different Pygmies for Pygmy Island, and there's only "
	"one I miss including - the Chief. "
	"I was considering him for boss of that world initially, "
	"but I'm happier with Kongor. A Chief couldn't have "
	"been that big really - especially if he was a pygmy! "
	"Watch the Hunter from a safe distance. He does a "
	"lovely dance. "
	"By the way, every time a pygmy talks, if it isn't "
	"saying \"Hey\", it's saying "
	"UNGA BUNGA.",
	// Pygmy Shaman
	"These are quite evil.  They're female by the way, "
	"they wear little bras.  They are actually more alert "
	"than the other Pygmies, which couples quite evilly "
	"with their poisonous voodoo lightning that can go "
	"right through walls.  Thankfully, they're a little "
	"more reluctant to use their power than other "
	"pygmies as well.  Sometimes one will simply not "
	"bother to attack you for a long time! "
	"Sometimes they will try to shoot lightning and it "
	"won't happen.  Go figure!  Their voodoo stick jams.",
	// Pumpkinstein
	"A never completed idea I had for SPISHulk, finally "
	"come to life.  An obvious choice for a pumpkin "
	"badguy in the Haunted Mansion!  These guys don't "
	"react to being hit at all.  What they do is run "
	"until they hit a wall, pick a new direction at "
	"random, and run at a random speed in that "
	"direction.  They don't even particularly care "
	"if they hit you, they just try to turn to a new "
	"direction and move on!",
	// Stan Glass
	"Sadly, that was the most clever name I could "
	"come up with for a stained glass enemy.  Notice "
	"how when he reflects your hammers, they are "
	"grey.  That's because he's actually turning "
	"them into the hammers that MechaBouaphas "
	"throw!  Less work than adding a new type of "
	"evil red hammer.  I'm very lazy.  But also, "
	"it's good for the player, so they can see "
	"the difference. I do think about these things "
	"sometimes you know! "
	"The best tip for fighting this guy is that "
	"he can't reflect anything but hammers.",
	// Triceroid
	"These work like Pumpkinstein, but with a "
	"twist. They start out very slow, but each "
	"time you hit them, they speed up. They "
	"also do a cute little roar when they bump "
	"you. These are monsters stolen out of "
	"Eddie Galaxy, by the way. I just thought "
	"they were so cute I wanted to reuse them "
	"in a context where you could see them up "
	"close.",
	// Countess
	"(bby)This is probably the easiest boss in the "
	"game. I think I made her pretty "
	"tough with the layout I made in the Haunted "
	"Mansion, but if you set her up like any "
	"other boss, with oceans of firepower at your "
	"disposal, she's no trouble at all! "
	"Her dash attack is the most dangerous thing, "
	"Otherwise, she just summons/shoots monsters "
	"at you, ones that are easy to beat and turn "
	"into more firepower for you to take her down! "
	"On top of that, they get in the way of her "
	"dashing at you. "
	"She's a pushover.",
	// Egg
	"(bby)This is obviously based on the movie Alien. "
	"You guessed that. It's also the 'mushroom' "
	"of the final world in Eddie Galaxy. And "
	"just as in Eddie Galaxy, it spews out "
	"Xenoids. They were VERY hard to see in "
	"that game though! "
	"When destroyed, the Egg will sit and spew "
	"acid for a short while.  Best to get away.",
	// Xenoid
	"Tinier than an Eensy Weensy?  Maybe! "
	"Certainly very similar to an Eensy Weensy "
	"in every respect, just more spacey. "
	"They also spew acid when killed, it just "
	"happens to be in tiny harmless particle "
	"form.",
	// Xeno Hunter
	"I hope you noticed that these guys wear "
	"bermuda shorts.  This is actually the model "
	"for the final boss of a game that was never "
	"finished called Mucho Kombat Vs. SPISPOPD. "
	"In that game he was said to be a demon, but "
	"of course, he was originally modeled after "
	"H.R. Giger type of art. And also of course, "
	"the bermuda shorts were added so nobody "
	"would accidently think I had gone serious "
	"on them! "
	"Run away quickly when you finish off a Xeno "
	"Hunter. That acid splash can hurt.",
	// Robopumpkin
	"These are big and nasty. But there "
	"are a few handy tricks. One is that "
	"their gun is off to one side of their "
	"body, so it is possible to trap them "
	"in a doorway so that they are shooting the "
	"wall, but think they're shooting you. "
	"Another is that they are big, so don't fit "
	"through small doorways, and they always "
	"pursue you relentlessly. A little known "
	"feature of them is their ability to grab at "
	"you with the metal claw they have on one side. "
	"It does a whole lot of damage. Don't get "
	"close to a Robopumpkin!",
	// Shock Trooper
	"Based on the Invid Shock Troopers from the "
	"3rd war in the Robotech cartoon series. A "
	"reference most will not catch!  This is "
	"actually a model I made MANY years ago, but I "
	"just couldn't come up with a secret enemy for "
	"the space world, so I looked through my files "
	"and found this. "
	"Turned out to be the most impossible secret "
	"level in the game! Sorry!",
	// Mini Bot
	"So cute. I like the robot noises I made."
	"In designing these (and Meaniebots), I was trying to make the "
	"most generic, clunky, sci-fi 50's robot I could. "
	"It was fun and easy, since it's mostly boxes! ",
	// Meanie Bot
	"(bby)These guys are tough!  They try to turn into "
	"two Mini-Bots when killed, but usually can't "
	"fit both into play (because other monsters and "
	"things are in the way). "
	"The name for these (which Mini Bot was then "
	"based on) is based on a stupid, obscure, in-joke "
	"insult from my high school days: meanie butt. "
	"The Mini Bot is of course just the head of this "
	"with wheels.",
	// Robo Factory
	"(bby)A fun little animation cranks out MiniBots. "
	"Kind of a shame to destroy these. I had to make "
	"the construction animation backwards, so that "
	"the finished product would precisely match the "
	"actual Mini Bot.  Wondering why some monsters "
	"have (bby) at the beginning of their description? "
	"It indicates that they make more monsters in some "
	"way.  It's handy for level designers to know.",
	// Turret
	"The best way to take these down is to stand "
	"farther away from them than they can reach - "
	"your hammers have more range than their bullets! "
	"If that isn't an option, there's a slightly "
	"trickier way. Get up closer, and move to the "
	"side bit by bit.  Eventually you reach a point "
	"where you are able to hit them with hammers "
	"still, but their bullets pass by you (hammers "
	"being wider than bullets).",
	// Doom Bunny
	"One of my favorites. I love their little waddle. "
	"What could be happier and less threatening than "
	"a Doom Bunny? Just do not harass them and "
	"everything will be fine...",
	// Kongor
	"(bby)Why does a giant ape look so much like a bear? "
	"Because I tried so hard to make a monkey face, and "
	"I just could not do it. He's another "
	"boss that poses no threat unless you are in front "
	"of him. Those fist pounds do cover quite a radius "
	"though. He should really only be used in very "
	"confined areas like the one you face him in in "
	"Pygmy Island, since not only is he harmless "
	"from behind, but he's also harmless at long range, "
	"and can only move sideways! When he pounds his "
	"chest, it summons pygmies offscreen.",
	// Squash
	"Everybody thinks they look like peppers. Well, "
	"they're NOT. They're SQUASHES. Get used to it! "
	"Why they run around with their mouths wide open, "
	"I don't know. They are a 3D representation of "
	"the original creature from SPISPOPD, which also "
	"had that unique trait I believe. "
	"Squashes are quite fast and deadly. A lightly "
	"armed Bouapha surrounded by them will soon be "
	"back at the beginning of the level.",
	// Ultra Pygmy
	"The cutest of all enemies, I say. They're called "
	"UltraPygmies because they are more pygmy than "
	"a pygmy. Simple, really. I don't know what "
	"else to say, but I really like these guys an "
	"awful lot.",
	// Loony Bot 5000
	"(bby)The most unique boss - it can only be hurt when "
	"it opens up the hatch in front to unleash the "
	"deathbeam. In the meantime though, you can "
	"destroy the turrets mounted on top. Listen "
	"carefully for what I think is a pretty cool "
	"sound effect when it's walking. ",
	// loony core
	"DANGER: Do not use unless you know what you are doing!  This is the invisible, damagable portion of the Loonybot.",
	// loony gun
	"DANGER: Do not use unless you know what you are doing!  This is a Loonybot turret.",
	// Loony Zoomer
	"When the Loonybot 5000 is destroyed, Dr. "
	"Lunatic tries to escape in his Loony Zoomer. "
	"The rapid fire capability is fairly serious, "
	"but mostly the Zoomer isn't much of a threat. "
	"It had to be pretty easy, since you'd have to "
	"refight the whole Loonybot if you got beaten "
	"by it! "
	"Look closely at the Zoomer and you will see "
	"that Dr. Lunatic has no arms inside it, and is "
	"actually encased in steel up to his chest. "
	"Poor guy!",
	// Buddy Bunny
	"My first idea for the fun pack - a monster that "
	"was utterly harmless and it was your job to "
	"protect.  Kind of like that old arcade game "
	"Crossbow!  Fun!  Anyway, they look nearly "
	"identical to Doom Bunnies.  That's because grey "
	"and light light brown were the only colors that "
	"they looked decent in.  Using Buddy Bunnies "
	"properly is pretty complicated - take care with "
	"them and test thoroughly!",
	// Good Turret
	"I really wanted the player to be able to 'summon' "
	"help, so I invented the Good Turret.  It's not "
	"that great as a helper, but you do get three of "
	"them per pack!  One of the saddest features of the "
	"game is the (remarkably easy) ability to permanently "
	"trap yourself using Turrets.  I thought of making it "
	"possible to walk through them, but I felt it would be "
	"more fun if you could use them as roadblocks as well "
	"as weapons.  When they are destroyed, their explosion "
	"can take out badguys.",
	// Reverse roly poly
	"I decided it would be fun and easy to add these in to the "
	"game.  They're only there for artistic purposes - if you "
	"want nice symmetrical patterns of Roly Polies, use reverse "
	"ones on one side, and normal ones on the other.  The only "
	"difference is that reverse ones turn counterclockwise when "
	"they hit a wall.",
	// Crazy Egg
	"(bby)I don't know what possessed me to create this monstrosity.  "
	"It is kind of fun though.  When I made it, I was picturing "
	"games like Smash TV or Gauntlet, where you have to plow through "
	"an endless horde of weak badguys to get to the device generating "
	"them (actually, that part isn't in Smash TV, but it sure is a "
	"fun game!).  Unlike regular alien eggs, these don't begin "
	"generating Xenoids until the player gets relatively near.  But "
	"once they start, there's only one way to make them stop!!",
	// Matilda-X
	"This mutant beast is a nod to those players who find all the "
	"ridiculously impossible levels to just plain be too easy.  "
	"I figured I would add something so that world builders could kick "
	"the difficulty up yet another notch.  "
	"Plus I had the realization that attaching the tentacles to Matilda "
	"instead of her claws wouldn't be very hard!  That got me thinking "
	"about a lot of other interesting crossbreeds, but in the end I decided "
	"to keep things simple.",
	// mat2skull
	"DANGER: Do not use unless you know what you are doing!  This is Matilda-X's skull.",
	// mat2brain
	"DANGER: Do not use unless you know what you are doing!  This is Matilda-X's brain.",
	// mat2body
	"DANGER: Do not use unless you know what you are doing!  This is Matilda-X's body.",
	// mat2tail
	"DANGER: Do not use unless you know what you are doing!  This is Matilda-X's tail.",
	// Pygmy Queen
	"(bby)With all the existing types, I just knew the Fun Pack had to expand upon "
	"the Pygmy repertoire.  The Queen isn't terribly dangerous, but it's fun how "
	"she makes such an enormous army of tiny warriors.",
	// Jalapeno
	"Because Squashes weren't nasty enough.  Don't let these guys get anywhere "
	"near you, because they never back off, and when defeated, they make a very "
	"painful explosion.  You have to take them out from a distance.  Luckily they "
	"share the Squash's limited intelligence and can be easily trapped behind corners.  "
	"But one mistake and you're fried, they move fast!!",
	// generator
	"(bby)Generators were one of the first notions I had in creating the Fun Pack.  "
	"I wanted the level designer to have the freedom to constantly generate monsters "
	"of whatever type they chose, at a rate that they chose.  The number on the generator "
	"name (which doesn't appear to the player, only in the editor) is how many seconds "
	"it waits between generations.",
	// generator2
	"(bby)See Generator-1S.  This one takes 5 seconds between monsters though.  The longer a "
	"Generator takes to make a monster, the more life the generator has.",
	// generator3
	"(bby)See Generator-1S.  This one takes 15 seconds to create a new monster.  A Generator "
	"that creates friendly monsters (Buddy Bunny, Wacky Wizard, HelpyBot, BuddyBot) will "
	"be friendly - immune to Bouapha's shots, and vulnerable to enemy shots.",
	// generator4
	"(bby)See Generator-1S.  Originally, this one was a full minute between monsters, but that "
	"was simply too long to be of any use!  Monsters that have (bby) at the beginning of their "
	"description are monsters that create other monsters.  Just a handy note for world builders.",
	// sneaky shark
	"This is the only monster from the Fun Pack, besides the Generator, which used all new "
	"graphics.  Well, not ALL new, since he does disguise himself as a raft.  In the editor, "
	"he appears red for easy identification, but in the game he looks just like any raft.  "
	"In creating this guy, I truly hoped to cause some mild heart attacks, so use him in "
	"unpredictable ways!  He does kind of look like a whale, doesn't he?",
	// mad millenium bug
	"Kind of a dull enemy in my opinion.  The only reason I added him was to have the name "
	"put in the game (from a poll on the forum, kind of a fun in-joke).  He is pretty darn "
	"tough for such a little guy (which makes him tougher still, since he's so hard to hit).",
	// wacky wizard
	"Another of the in-joke names, this guy was tough to come up with.  But he's not a "
	"friendly Santa Claus!!  He's a WIZARD!  And he's WACKY!  He's also very powerful.  "
	"Use him sparingly in your levels, as he can take out most enemies pretty handily.  "
	"He's about equivalent to a Super Zombie.  When he shoots those big bullets, those "
	"are supposed to be his deadly eyebeams - there's two of them and they come from his "
	"eyes down to the floor.  He also randomly uses a Reflect Shield when hit.",
	// evil clone
	"I wanted something more Bouaphaey than MechaBouapha, so here comes "
	"the Evil Clone!  They even have the exact "
	"same amount of life as Bouapha himself - 128 (for reference, Bouapha's hammers inflict "
	"5 damage, and a Mama Spider has 200 life).  I had considered letting them throw red "
	"hammers instead of the evil grey ones, but it was simpler to use the grey ones, and "
	"less confusing for the player.  They also originally used Bouapha's own sounds, but "
	"it was very disorienting every time you defeated one!",
	// bob the biscuit
	"(bby) The last of the in-joke monsters. This is the Fun Pack monster I'm least pleased "
	"with. He's very uneven - only slightly harder to beat than an ordinary Ninjabread Man, "
	"very hard to tell apart from them visually (he was very different looking originally, "
	"but it looked bad having him bright yellow! So instead he's just a burnt Ninjabread "
	"Man), and yet he can practically wipe you out in one attack if he's lucky. Bob is "
	"just, well, kinda lame. But like the Mad Millennium Bug, I couldn't leave him out!",
	// multimoss
	"(bby)The Multi in their name stands for Multiplying.  The concept behind these guys is very "
	"simple - an enemy that you simply CANNOT attack, because if you do, you will suffer.  "
	"Sure, you could say that's what Doom Bunnies are, but you can eventually wipe them out.  "
	"MultiMoss just keeps growing, and growing, and growing...  Kind of a 'high-concept' "
	"monster, one I hope world builders will take and do interesting things with.",
	// moss rapido
	"(bby)Couldn't miss out on using this name, now could I?  This, along with the Crazy Egg, was "
	"my attempt to include some really fast-paced gameplay into Dr. Lunatic if the world "
	"builder wants to include it.  I don't know if it did that, but it's a fun monster anyway.",
	// snowball
	"This was a request by a world builder (one of many, I don't remember which all features "
	"were conceived by world builders!) - a little artistic freedom with Roly Polies.  To add "
	"to the fun, the Snowball will explode on contact with water or lava, making it more "
	"useful for various puzzly purposes than a regular Roly Poly.",
	// snowball2
	"Just like a Reverse Roly Poly, this is just a Snowball that turns counterclockwise "
	"on impact.  Simple enough!",
	// snowblower
	"A thematic variation on the Burner.  It's not terribly dangerous, I've found, or "
	"really that much fun usually, but it makes interesting noises.",
	// boomkin
	"Nothing better than variety in Pumpkins!  I tried to add a little something to each "
	"'family' of monsters in Dr. Lunatic.  This is one of the enhancements to the pumpkin "
	"family.  When they get close, they self destruct, and the damage is horrifying!  "
	"Even if you shoot them yourself, they still blow up, so you have to do it at a "
	"distance.  You just can't let them get near you at all, or it's about 40%"
	"over!  Two Boomkin hits is remarkably close to fatal.",
	// manic mumble
	"The exact opposite of a normal Mumble - very little life, very very fast"
	"moving!  Not to mention the intensely bright aqua color.  They look very manic, "
	"and they are.  The sound when they try to repeatedly smash your head at light "
	"speed is pretty funny.",
	// good robot
	"Another helpful monster!  These are quite weak.  They're exactly equivalent "
	"to the enemy MiniBots, only they try to help the player out.",
	// good robot 2
	"(bby)The big brother to the BuddyBot.  Exactly equivalent to a MeanieBot, but friendly.  "
	"Originally I was testing these out and couldn't figure out why they would always "
	"lose in a big war with MeanieBots.  Until I realized they released MiniBots "
	"when defeated instead of BuddyBots.  D'oh!  That is fixed now, so they are on "
	"nice even footing.",
	// xeno queen
	"(bby)The xeno family needed a more top-of-the-line member, and so the first thing "
	"that came to mind was the queen from Aliens.  So that's sorta what this is - "
	"a Xeno Hunter that lays eggs.  It does so a lot, and can be very troublesome "
	"in the right environment.  It also is quite tough to beat, and you definitely "
	"don't want to be nearby when one dies!  They combine the instant splash of the "
	"Xeno Hunters with the continuous spray of the Xeno Eggs.  Highly dangerous lifeform.",
	// rumbly tumbly
	"The easy version of the Roly Poly.  It is lighter in color, though the difference "
	"may not be immediately obvious if you don't see them next to each other.  It "
	"takes only a few hits to destroy a Rumbly Tumbly, but they're still quite "
	"dangerous, since they are deadly on impact, and explode into flying boulders "
	"when destroyed.",
	// rumbly 2
	"A Rumbly Tumbly that turns counterclockwise on impact with a wall or monster.",
	// dark vampire
	"A very unique and deadly adversary!  Originally I thought I might give them "
	"even more powers, so they would really fit most of the legend of vampires "
	"(for instance, the ability to become bats, though if they did, they'd sure "
	"be easy to beat!).  Instead I went for the simple answer - invincible, "
	"but damaged by light.  Even if you have them in the light, you need to be quite "
	"careful around these guys - they have deadly poison claws!",
	// grabby gnome
	"The most fun of all enemies!  These guys try to steal Bouapha's weapons.  "
	"They can take Pants, Hammers, Sproingy Springs, or Reverse Hammers.  Each "
	"Gnome can only hold one item, and once he has one, he will do his "
	"best to run away from Bouapha with it.  If Bouapha has no hammers, "
	"the Gnomes simply smack him with their tiny sticks.  When a Gnome is beaten, "
	"it drops whatever it had stolen.  I love these guys, they make the game fun.  "
	"They also have cute noises.",
	// nobody
	"no notes",
	// determined bunny
	"This is another form of the Buddy Bunny.  In the game, the player just sees \"Buddy Bunny\" "
	"for the name.  Unlike the normal Buddy Bunny, a "
	"Determined Bunny never strays from its path!  A Determined Bunny is suited "
	"more to specific puzzles that require an exact solution, while the Buddy "
	"Bunny is more freeform.  The truth is, the Determined Bunny is vastly more "
	"useful, and you may never use a regular Buddy Bunny at all!  But "
	"they're good to have for crowds of civilians or something like that.",
	// rock trooper
	"On my original list of things to do for the Fun Pack was to make a less "
	"dangerous version of the Shock Trooper.  But this didn't actually arrive "
	"until the very last day of development.  But here it is!  Easier to beat, "
	"slower, and shoots a much less dangerous weapon at you.",
	// jacko
	"These guys are so creepy, silently sliding along the ground toward you... "
	"they are intended to be the equivalent of the Big Pumpkins in the original "
	"SPISPOPD game.  Just like those guys, they spew out regular pumpkins when "
	"beaten.  A random number in this case (always 2 in the original game).  "
	"Interesting note: this is actually the head of the Centipumpkin, if you didn't notice.",
	// crazybone
	"(bby)Another initial idea for the Fun Pack was to make more varieties of "
	"Boneheads.  This is the only one that actually arrived.  It does more damage "
	"than a Bonehead (due to those big fat bullets) and moves faster and is "
	"generally more crazy.  The craziest thing of all of course is that "
	"there is a chance (2 separate 1 in 6 chances if my calculations are correct!) "
	"that they'll get back up when beaten.  Makes a cool sound too.",
	// creepazoid
	"And of course, there had to be a new form of Zoid or the Fun Pack would've "
	"been no fun at all!!  This was the only thing I could think of for a Zoid "
	"that would be different from the others - a ghostly one that moved through "
	"walls and spit poison gas.  Frankly, I find them quite undangerous, but they "
	"are a little creepy.",
	// lazybones
	"The very first monster added to Supreme With Cheese!  I thought I wanted a new "
	"type of Bonehead, and the first phrase that came to mind was Lazybones.  The way "
	"they function pretty much followed from the name. A room full of these guys can "
	"be quite frightful!  They wake up whenever Bouapha gets close, or they get injured.",
	// starey bat
	"I was really stuck for clever names for a new type of bat.  The only good one "
	"I could think of was Baseball Bat, but I didn't want to have to draw a whole "
	"new one.  So you get the Starey Bat. "
	"It's for use in puzzle type situations, or just to totally annoy the player. "
	"Unlike the Dust Devil, you can't overcome the psychic stare of the Starey Bat, "
	"so feel free to use it to completely block in the player.  It works through walls, "
	"so you can make it invincible if you need to!",
	// madman
	"Just another type of Wacko... these ones are half as strong, but move faster, "
	"and chase you relentlessly like a Pygmy (only without the resting!). Not the most "
	"exciting enemy, but they add to the variety and splendor that is Dr. Lunatic!",
	// pygmy acct.
	"The first monster made specifically for the Urban setting.  Even Pygmies need to "
	"pay their taxes, so they hire Pgymy Accountants to do them.  Note that he wears a "
	"dark blue business mask, rather than one of the more casual multi-colored varieties. "
	"These guys have more of an attention span and spotting radius than your average "
	"Pygmy, and they're capable of inflicting much more damage as well.  On top of all "
	"that, they have more life!",
	// mr. crazypants
	"The crazed leader of the Wackos.  I really love his sound effects and attacks.  One "
	"of my favorite enemies.  And he seems so easy at first, doesn't he?  Take advantage "
	"of the brief moments when he yells at you each time you 'beat' him, that's your "
	"chance to take a couple free shots.  But get out of the way quickly, because those "
	"windmilling blades will be coming in real fast!",
	// yerfdog
	"This isn't really a monster so much.  It's our company mascot!  I just made him a "
	"monster instead of an item for the added cuteness of seeing him hop up and down. "
	"He doesn't actually do anything at all in the game - he's completely nonexistent, except "
	"that you can see him.  What I use him for is the provide messages to the player.  There's "
	"even a special background for text messages that you can use just for him!",
	// elite seal
	"Not the most intriguing new monster in the world... it's a SEAL that shoots missiles! "
	"It's also tougher than a normal Baby SEAL.  The missiles are quite dangerous actually. "
	"I recommend you put groups of Baby SEALs in your levels, with an Elite SEAL as their "
	"leader.  That's just classy!",
	// creeping cone
	"Look around the next time you're driving on the freeway.  Notice how orange cones just "
	"lurk all around.  They're in the most unexpected places.  And sometimes they appear to "
	"be hiding... or plotting... We have a theory that in fact they are aliens plotting the "
	"annihilation of our entire species.  This paranoid fantasy lends itself to these unique "
	"little monsters.  They are the only enemy in the game that does absolutely nothing until being "
	"attacked.  Taunt them all you like!",
	// pygmy diver
	"Pygmies beneath the sea.  There's no escaping them!  There's nothing too special about "
	"them other than that they swim.  They act a lot like Pygmy Hunters otherwise.",
	// puffyfish
	"Can't have underwater levels without some form of inflatable fish.  These guys puff up "
	"if you get too close, or if you shoot them.  Other than that, they simply swim in circles. "
	"I kind of like that for some reason.  Like most enemies that fire 'cactus spines', these "
	"guys can hurt you bad!",
	// reverse puffy fish
	"This is the same as a normal Puffyfish, only it swims the other way around.  Thought that "
	"would be nice for some variety!",
	// olaf
	"The long-lost yeti brother has returned!  Olaf is in some ways the scariest of the brothers. "
	"He fires the Snowball monster, which is instant death if it touches Bouapha.  When putting one "
	"of these in a level, PLEASE be sure to put him on an island!  If he's in an enclosed room, "
	"those snowballs are going to stack up and make life very bad for Bouapha.  Because his weapon "
	"is so lethal, Olaf yodels more often than the other brothers, and has half as much life.",
	// spikey
	"This particular vampire is based, exceedingly loosely, on a popular fictional vampire "
	"from a television series which is one of the "
	"greatest TV shows of all time, alongside Firefly, Angel, The Daily Show, The Simpsons (before "
	"it got crappy), Futurama, and a couple others that obviously must not be quite as great, or "
	"maybe just not as recent, since I can't think of them offhand.  Anyway, you figure out who it's "
	"based on.  Sorry it looks nothing like him, but I didn't want to make new graphics for it!",
	// bombot
	"The name pretty much covers this guy... A robot that throws bombs.  It's definitely the scariest "
	"of the standard bots.  It's even got more life than normal!  It's red because red=danger.",
	// follow bunny
	"Another fun flavor of Buddy Bunny!  This one is my favorite, and I have to say the 'AI' on it "
	"is pretty good... it looks like they're actually intelligent (well, unintelligent, but conscious). "
	"And they don't block you in much either, because they automatically get out of your way.  These guys "
	"could be used to make some righteously fun levels, so get to it, world builders!  Rescue the bunnies!",
	// autozoid
	"My original idea for a Zoid for the urban environment was one that popped out of manholes, possibly "
	"a Sewerzoid.  But I thought this was... well, I don't know, just did this instead!  These guys are "
	"not dangerous unless they're in a very wide open area.  If they hit a wall, they explode!  They're "
	"not much like any other zoid, but they're fun and interesting.  PS - they can smash minecart barriers!",
	// treasure chest
	"This isn't much of a monster at all!  The only reason it counts as a monster is so that it can take "
	"multiple shots to destroy.  Items can only be one hit to destroy.  Use treasure chests as a cute way "
	"of placing items in your levels.  The only real trick you can use there is that of course, if the "
	"player is unarmed, they can't get the item, so you can build some mini-puzzles around that.",
	// Jellofish
	"It's supposed to look like a cube of jello, jiggling.  I tried putting banana chunks in it, but that "
	"looked very odd.  The electrical attack is just taken from the way jellyfish are used in the cartoon "
	"Spongebob Squarepants, as some sort of weird electrical bees. I'm really disappointed at how hard it "
	"is to see their jiggling as they move, especially underwater where they belong.  Oh well, they're just "
	"evil cubes!",
	// King Cone
	"(bby)The leader of the evil cones.  Note his evil mustache, and the crown which proves he's a king.  For "
	"each original world, I always had a type of zoid, a boss, a type of pumpkin, and a wacky secret enemy. "
	"So with the two new themes in Supreme With Cheese, I decided to follow the same formula, and this is "
	"the wacky secret enemy for the urban theme.  Wacky!",
	// Mailboxer
	"One of the first ideas I had for an urban enemy - just take the word mailbox, and make it a little more "
	"action packed.  Unlike Creeping Cones and Sneaky Shark, this is a similar-to-an-item monster that is not "
	"afraid to show its true face.  It was never intended to be tricky, just an interesting notion for "
	"something to battle in an urban setting.",
	// Piranha
	"These are a little weirder than I had planned.  I'm not sure exactly what's off about them, but they're "
	"just not quite what I envisioned.  Still, it's fun to see a huge school of them swarming Bouapha.  In "
	"case you didn't notice, they are actually just hyperspeed Pengulons that don't wander randomly and do "
	"half damage!",
	// You-Go
	"What a fun way for Bouapha to get around town!  I really enjoy driving this little thing.  Be aware that "
	"anything a minecart can smash, a You-Go can too.  And if you are putting these in your own level, you "
	"should know that they can only drive on terrain marked 'minecart path'.  However, they have free reign "
	"on it, unlike a minecart.  If they hit normal ground, Bouapha's immediately ejected.",
	// minisub
	"DANGER: Do not use unless you know what you are doing! "
	"This is the dummy monster for Bouapha in the Mini-Sub.",
	// punkin
	"Oy oy oy!  These punks are supposed to be British soccer hooligans.  My friend Ralph used to listen to "
	"that kind of punk music that pretty much consisted entirely of \"OY OY OY OY!\", so I thought it was "
	"appropriate.  See, the punks just stand around, then charge at you shouting.  Just another type of "
	"pumpkin to enjoy!",
	// sea cuke
	"This model was originally in SPISHulk, which is just too insane to describe.  He was used in a side-view "
	"fighting game type of capacity.  That's why he often seems to be standing sideways to the direction he is "
	"attacking.  I should've changed that, but I thought it would look right!  I don't really know why he has a "
	"cane, spats, and a top hat.  It just seems appropriate.  He's cool as a cucumber.  In SPISHulk, he wasn't a "
	"sea cucumber though, so the snorkel and mask are new.",
	// sneaky shark: chest
	"Sneaky Shark is always trying to surprise you!  Of course, this disguise is not as effective as the raft "
	"one... after all, you're SUPPOSED to shoot treasure chests, so you're not as likely to get surprised.  Still "
	"though, he's a crafty one, ain't he?",
	// starfish
	"Not the most clever name or design ever, but they're interesting in their behavior. As long as you stay near "
	"a starfish, it will never stop shooting.  Very violent little critters.  Other than that, they just wheel "
	"around randomly.  However, if you hurt them, they immediately wheel towards you in anger!  Beware the starfish! "
	"It's possible, but quite challenging, to walk in a circle around a starfish between its shots.",
	// underzoid
	"See, when you're underwater, you can finally see the whole body of the Aquazoid!  It's kinda disappointing "
	"actually.  I wanted the body to be REALLY tiny!  But if I had done that, the neck wouldn't have been able "
	"to match up with it, now would it?  That thing's got a thick neck.",
	// wet seal
	"Just a silly name I thought of, and seals do swim in the ocean, so this is the underwater version of the Baby "
	"SEAL.  It's really not much different at all, other than its ability to float over obstacles.  Which really "
	"makes you wonder why Bouapha can't get up off of the ocean floor!  The answer is simple: look at those boots!",
	// wet willy
	"When hydrants go bad.  I think this one was more a case of brainstorming on ideas for urban enemies and liking "
	"the picture I drew (which can be seen elsewhere in this product somewhere...).  The end result is not far "
	"removed from the concept sketch.  The animation he uses to run around with was actually intended to be his "
	"standing still animation, but it took up so many frames I decided it would do for running around too.  After "
	"all, he's a rabid little freak!",
	// crab puff
	"These are one of my favorite enemies.  They're so completely basic, but they just pour out after you in a horde! "
	"There's no tricks or anything to them, just hammer away!",
	// patch
	"(bby)Sure, you could use a Generator to constantly make pumpkins, but what's the fun in that?  The pumpkins created "
	"are immediately 'awake', and facing a random direction, so that the patch will have room to keep creating pumpkins "
	"for quite a while, even if there is no player around to lure the pumpkins away.  It makes pumpkins once every 5 "
	"seconds, if you were wondering.",
	// fast patch
	"(bby)This is identical to a regular Pumpkin Patch, except for a slight tint to the color, and the fact that it makes a "
	"new pumpkin every second!  I like the rustling animation the patch does when it's ready to hatch a new one.",
	// boom patch
	"(bby)The scariest of patches, this one will generate a new Boomkin every 5 seconds.  Use sparingly, much like chili paste.",
	// squash patch
	"(bby)Yet another form of the Pumpkin Patch, this one makes Squashes, one every 3 seconds.  Aren't Squashes annoying?  And "
	"what's with that bubble the new enemies come from?  Weird!  That was really just so I could use the same graphics for "
	"patches that grow different enemies.  I think it actually works pretty well.",
	// patty
	"(bby)Okay, so there are two odes to Spongebob Squarepants in this game.  Now I'm not really a big fan or "
	"anything.  In fact, I yell at the TV constantly when I watch it, because I can't stand how they randomly decide whether "
	"it will actually seem to be underwater or not (like he takes a bath... what is he soaking in, "
	"mercury?!?).  Drives me nuts... not that the underwater levels in Dr. Lunatic are much more accurate!  She's not a very "
	"tough boss, but she can be a nuisance with all her babies.",
	// dozer
	"The name of this monster is a reference to something.  I leave it as an exercise to the reader to figure out what.  Coming "
	"up with a boss for the urban area was tough.  This was my third idea! First it was The Mayor, an Uncle Sam "
	"type guy with rockets, then Loy-Ur, who would shout \"Habeus Corpses!\" to summon Boneheads. "
	"But I decided I really wanted another 'monstrous' type boss, huge and imposing. "
	"Dozer is some sort of evil anti-environment machine, chopping trees down and causing pollution. SO GO GET HIM!",
	// dark coffin
	"(bby)A coffin... except it makes a Dark Vampire instead of a normal one!  What a crazy wacky twist!  Oh man, I hope I didn't "
	"blow your mind right out of your head!",
	// Mutant Zombie
	"Oh no!  A Zombie has tromped through a pool of toxic waste!  A little more interesting than a normal Zombie, and more "
	"importantly - MUCH more dangerous!  Also note that its life regenerates over time.  Notice: if you swim in toxic waste "
	"yourself, you will NOT develop regenerative powers.",
	// whatsit
	"What better name for another Thingie than a Whatsit?  Probably lots of better names, but that's the one I picked. "
	"These are sort of more dangerous than a Thingie - they have more life and shoot more dangerous bullets, and shoot much "
	"more often.  But they're very slow-moving.  Just a little more Thingie flavor to round out the sparse line-up of "
	"Thingie-type enemies.",
	// shocking horror
	"Eh... shocking horror.  Get it?  Yeah.  Well, whatever.  Dangerous in large numbers or tight hallways.",
	// Mechazoid
	"Always good to have more zoids... this one is for outer space.  It doesn't look very mechanical, but there's only "
	"so much you can do by recoloring existing monsters!  Some people might complain, \""
	"Why'd you make all these lame monsters that are just recolored versions of other monsters?!  You're lame!\" "
	"To those people I say, what other games have you played lately with 200 different types of monsters?  And more of "
	"those are unique than are variants, so that's pretty crazy.  I go a little nuts making games.",
	// Sucker1
	"Sucks the player in from anywhere in the world.  Use it to simulate a horrible inescapable vortex of doom!!",
	// Sucker2
	"Sucks harder than a regular sucker!  Suckers and blowers are visible in editing, but in gameplay, they are invisible "
	"and can't be hit or touched in any way.",
	// sucker3
	"Sucks the same as a regular sucker, but only sucks over a limited radius.",
	// sucker4
	"Sucks harder than a regular sucker, over a limited radius.",
	// blower1
	"Whereas the Sucker sucks, this blows.  At first, I thought \"why do we need a Blower?  Just put a Sucker on the "
	"opposite wall, and it's a Blower!\", but I realized, it's all a matter of angles.  A Blower is indeed somewhat "
	"different from a Sucker.  Use whichever is appropriate for your level!",
	// blower2
	"Blows harder than a regular blower!",
	// blower3
	"Blows the same as a regular blower, but over a limited radius.",
	// blower4
	"Blows harder than a regular blower, over a limited radius.",
	// beetle
	"Don't you just love The Beetles?  I do.  Tiny, fast, low damage, easy to squish, in big hordes.  That's entertaining "
	"gameplay!  Watch them go on tour all over your ankles!",
	// scarab
	"(bby)The big mama to all the Beetles.  Not much to say about this fairly ordinary mini-boss.  I like it though.  Fairly "
	"classic in her fire-breathing, beetle-spewing design.  The idea to create these monsters actually came from a so-so "
	"Playstation game called Haven, which in turn got its inspiration VERY blatantly from the movie The Mummy.  I decided "
	"the desert needed scarab beetles!",
	// frostbiter
	"The most cold hearted of the spiders.  Also the only enemy (as of this writing!) that can freeze Bouapha.  Luckily for "
	"you, Bouapha, unlike monsters, is immune to being frozen further if he's already frozen, so you will eventually unfreeze! "
	"Still, it would be very unpleasant to have to travel through a room full of these suckers.  Also, being hit by anything "
	"else cuts your remaining freeze time in half.",
	// missile turret
	"Like a regular turret, only it shoots the same homing missiles Bouapha can.  Pretty dangerous, actually!",
	// death turret
	"The ultimate in cutting edge turret technology, or TT, the Death Turret is designed as a failsafe anti-personnel device. "
	"Nobody who comes within about a half-screen's length from it has ever returned to tell the tale.  Use these in Stealth "
	"levels to really force the player to be stealthy!  Because if Bouapha is seen by one of these, it's over.  It can be "
	"destroyed just as easily as any other turret, if you use a long range weapon or paralyze it somehow.",
	// magmazoid underwater
	"If you're in an 'underlava' level, then of course, you get to meet the entire body of the Magmazoid.  If you find it "
	"odd that they lack the delightful yellow spines of their above-lava brethren, you need to understand, those are special "
	"adaptations the Magmazoid has for concentrating heat from the atmosphere to keep it from cooling down too much (sort of "
	"the opposite of similar fins on other creatures) when it is out of the lava.  They retract underlava.  Riiiiiight.",
	// parked car
	"This is a parked car.  Think of it sorta like the barrels in Doom - blowing it up is kinda dangerous, but that's all.  It's "
	"a decoration to make an urban environment classier.",
	// parked car2
	"These people park very badly.  Each car gets a random facing when it's created, and that's it.  More visually interesting "
	"than having them all line up nicely!",
	// traffic
	"Don't play in traffic!  It'll mow you down like crabgrass.  This enemy works like a Roly Poly, except instead of going until "
	"it hits a wall, it follows a set path of tiles (ones marked 'Minecart Path').  This particular version turns clockwise if "
	"given a choice.  This is one of the very few enemies that is dangerous to both goodguys and badguys.",
	// traffic2
	"This color of car turns counterclockwise when given a choice.  Otherwise, it follows the path set before it.  Don't stand "
	"in its way.  It hurts.",
	// playable mechabouapha
	"DANGER: Do not use unless you know what you are doing! "
	"This is the dummy monster for the playable Mechabouapha.",
	// shroomlord
	"Ripped straight from today's headlines... or possibly Kid Mystic!  This is the big brother to the Shroom, and the little "
	"brother to Mush.  Just more fun in the wacky Shroom family.",
	// scarier bat
	"What's scarier than a Scary Bat?  A bigger bat that spits out Scary Bats!  If you're a true Hamumu fan, you'll recognize "
	"these guys from their first appearance in Kid Mystic.  I wasn't going to copy things over from there, but there was an insane "
	"outcry for Scarier Bats... I just don't know why!  But enjoy!",
	// die uberbat
	"The name of this bloated critter is German for \"The Superbat\", but uber is fun to say.  He looks like a muppet. "
	"The concept for this creature was probably born in the Stephen King short story Graveyard Shift, about giant rats, from "
	"the collection \"Night Shift\" I believe. If you read it (don't if you are easily creeped out!), you'll see the connection. Also "
	"a little Lovecraftian flavor there.  But not as much as The Thing!",
	// werepuppy
	"I think we ALL know where this monster came from.  They're so cute though!  I feel guilt taking monsters from my other "
	"games, feeling like that dilutes the unique value of this game, but on the other side of the equation we have the 180 or "
	"so completely original Dr. L monsters, and this is supposed to be the big Hamumu fan package, so it's okay! (PS - If you don't know where "
	"this came from, go buy Loonyland from our website right now!!  You're missing out!!  And I want your money!)",
	// weremutt
	"The more powerful cousin to the Werepuppy.  This is the only enemy capable of turning on the same super speed that "
	"Bouapha uses with his Particle Accelerator (the mutt does it by howling).  He also possesses a nasty charge attack "
	"that actually chases down the player, in addition to the traditional, adorable, werewolf pounce.",
	// Patrol-LR
	"In conjunction with the new Stealth levels, I wanted enemies that acted like the badguys in Metal Gear Solid - "
	"not noticing you unless you were right in front of them.  I find it fun trying to sneak around them.  Sadly, you "
	"can't sneak up behind them and grab them to use as human (mummy?) shields, but it's still fun!",
	// Patrol-UD
	"This is identical to the other Patrol Mumble, this one just patrols up and down instead of left and right!",
	// Death Patrol LR
	"If just lighting the player up isn't unpleasant enough for your particular level design, send in the Death Patrol. "
	"They are the Mumble equivalent of Death Turrets!",
	// Death Patrol UD
	"At least as exciting as the other version of the Death Patrol, these guys move up and down instead of left and right. "
	"Can't beat that with a stick!",
	// Pygmy Weatherman
	"This variant monster was invented by Master Jobee.  Creating it was his prize for having the most creative entry in the "
	"Dr. Lunatic Fan Art contest.  You can see his entry in the Gallery if you unlock it!  His entry also inspired something "
	"else.  You'll figure out what that is when you see the picture!  Anyway, this is the most powerful of the Pygmies, armed "
	"with the power of every type of weather imaginable.  Well, at least a bunch of them.",
	// hunter egg
	"This is the futuristic version of a Vampire Coffin!  Shoot it, or get too close, and a Xeno Hunter is hatched.  Doesn't "
	"it look too big to fit in the egg?  They're very flexible.",
	// playshroom
	"DANGER: Do not use unless you know what you are doing! "
	"This is the dummy monster for the playable Stupid Shroom.",
	// punk bunny
	"This monster was designed by Jack Darby.  Making it was his prize for making the funniest entry in the Fan Art contest. "
	"It's a very cool monster, probably the most useful friendly monster of all.  Not nearly as powerful as a Wacky Wizard, but "
	"it follows you around!  You just have to wake it up first.",
	// lunachick
	"DANGER: Do not use unless you know what you are doing! "
	"This is the dummy monster for the playable Lunachick.",
	// skeleton
	"This was a customer suggestion.  Get it? TON?  It's heavy.  Slow.  And makes the screen shake and whatnot.  It's pretty tough "
	"to beat, because after all, it's made of metal.  Or something.  It's heavy anyway, did I mention that?  It combines the "
	"ground pounding attacks of Super Zombies and the Super Duper Zombie into one vicious combo.  I like when it just starts "
	"hopping up and down really fast... it looks so mad!  Just a random occurrence.",
	// hot shroom
	"For some reason, there was a fair amount of clamor for a firebreathing variation on the Shroom.  I don't know exactly what "
	"the appeal is, but here we have it.  It's a shroom that shoots fire instead of poison.  The name just came to mind for "
	"some reason, I guess because hot & sour soup is about the only thing I'll eat with mushrooms in it.  These are a little "
	"faster and obviously much more dangerous than normal Shrooms.",
	// minizoid
	"Take anything big, and make it smaller.  BAM!  Instant cuteness!  It's just a rule of nature.  So in the grand tradition "
	"of the Ultrapygmies, we present the Microzoids.  Not much else to them.  They look like yellow Geozoids, but they live in "
	"water like Aquazoids.  Sorry, there's no underwater version of them.",
	// miniwacko
	"I always like minified versions of the normal monsters.  They're so cute!  These guys died their hair green to help cope with "
	"their newfound inferiority complex.  They're more aggressive and much crazier than their bigger cousins, but that's okay, because they "
	"are armed with some sort of knitting needle!  Not that dangerous, until they swarm you.",
	// jack frost
	"He's nipping at your nose!  He does that by firing freeze bolts at random.  For a mini-boss, this guy is pretty harmless. "
	"But if you want to make him tough, put him in fairly tight quarters with Bouapha and some other enemies!  That makes his "
	"ice shots hard to avoid, and greatly improves his odds of getting a swipe at you on his highly predictable journey.",
	//Pterodactyl
	"The level one summon spell from Kid "
	"Mystic. They're friendly, but not very "
	"strong. A small swarm of them can be "
	"helpful in a pinch, though. (SWE)",
	//Eye Guy
	"This guy is actually from Eddie Galaxy. "
	"With all of those eyes you'd think he "
	"would fire multiple projectiles at you but "
	"he doesn't. Still not a foe to take lightly. (SWE)",
	//Octon
	"These guys spend most of their time just "
	"below the surface of the water. You can "
	"only damage them when they shoot at you "
	"so be careful. (SWE)",
	//Submerged Octon
	"",
	//Blockhead
	"These little guys are weak but relentless. "
	"Not very deadly either on their own but "
	"quite dangerous in swarms! They'll nip at "
	"your HP so be sure to keep them at a "
	"distance. In Kid Mystic all of these golem "
	"enemies guarded an ancient maze-like "
	"dungeon that you had to navigate. (SWE)",
	//Blockhead Kiln
	"A hyperactive factory that constantly "
	"pushes out Blockheads. Be sure to take "
	"them out quickly or they'll never stop "
	"coming! According to Jamul, there's an "
	"elaborate system of gears modeled "
	"underneath that animates as it works. "
	"Unfortunate that you never get to see it. "
	"(SWE)",
	//Trapazoid
	"A pun that writes itself. How could there "
	"NOT be a \"Trapazoid\" somewhere in the "
	"Hamumuverse? It surprises you by leaping "
	"out of a sewer and spewing fire at "
	"you, hence the name. It's a bit like the "
	"Sneaky Shark in that respect. (SWE)",
	//Vulnerable Trapazoid
	"",
	//Megablockhead H
	"These guys are deadly and invincible, so "
	"watch out! Since they shoot lasers, their "
	"field of vision is narrower than a Death "
	"Patrol so keep this in mind. (SWE)",
	//Megablockhead V
	"The same thing but walks in a vertical "
	"pattern. (SWE)",
	//Slimy Slug
	"They creep around leaving a deadly trail "
	"of slime in their wake. Their slime "
	"disappears on its own after awhile, though "
	"so don't worry. Though if you're facing "
	"many of them at once you might just have "
	"to wait awhile. Try placing them in "
	"smaller areas for more of a challenge. "
	"(SWE)",
	//Sniper Snail
	"They only shoot at you if you're far away. "
	"If you get too close, they shrink into "
	"their shell and turn invincible! (SWE)",
	//Sneaky Snail
	"",
	//Oculoid
	"These guys are tough! The projectile they "
	"shoot bounces around so make note of that "
	"when you design your levels. Narrow "
	"hallways make these things a beast! These "
	"things also originate from Eddie Galaxy. "
	"(SWE)",
	//The Lookey-Loo
	"This chapter 3 boss isn't as dangerous as "
	"it appears, though it can only be damaged "
	"if its main eye is open. In Kid Mystic, he "
	"was flanked by many Pinkeyes which were "
	"constantly regenerating as the fight went "
	"on. (SWE)",
	//Pinkeye
	"They aren't very strong on their own, but "
	"they DO come back after a bit so be "
	"careful. In Kid Mystic, they guarded the "
	"Lookey-Loo at the end of chapter 3. ",
	//Crazy Bush
	"Like the Blockhead, but even easier to "
	"kill. Just because these crazy looking "
	"guys were found in Chapter 1 of Kid Mystic "
	"doesn't mean you should take them lightly "
	"though. With a big enough swarm and "
	"low enough firepower anything can be "
	"deadly enough. (SWE)",
	//Peeping Bomb (CW)
	"Since Kid Mystic didn't have a stealth "
	"mechanic, you had enemies like this that "
	"only chase you if they're looking directly "
	"at you. Be sure to take them out before "
	"they spot you, they're really fast! "
	"(SWE)",
	//Rammy
	"One of the three Gruffs from the end of "
	"chapter 2. This one in particular charges "
	"at you relentlessly, and is only "
	"vulnerable for a short time when he hits "
	"something. (SWE)",
	//Vulnerable Rammy
	"",
	//Hoppy
	"This one repeatedly jumps up into the air "
	"causing deadly shockwaves when it lands. "
	"Naturally, it can only be damaged when "
	"it's on the ground. Each Gruff isn't too "
	"difficult on their own, but try fighting "
	"all three at once! (SWE)",
	//Invincible Hoppy
	"",
	//Gruffy
	"One of the three Goats who block your way "
	"at the end of Chapter 2. Each of them acts "
	"in a unique way, with this one "
	"periodically firing bouncing orbs at you. "
	"In the original eGames version of Kid Mystic, "
	"you were faced with Mush the "
	"Shroom instead. (SWE)",
	//Happy Stick Shroom
	"These guys are a bit derpy looking, but "
	"how would YOU draw one? I think it's part "
	"of the charm! These guys from chapter 1 "
	"are probably the easiest of the bunch. "
	"(SWE)",
	//Happy Stick Spider
	"Spews almost as much acid as a Mama "
	"Spider, and is much faster as well. This "
	"guy was native to chapter 2. (SWE)",
	//Happy Stick Corpse
	"According to Jamul, the \"Canadian\" look of "
	"these guys was unintended, and was just "
	"the result of trying to draw a stick "
	"skeleton. Since they're from chapter 3, "
	"these guys can be quite dangerous! (SWE)",
	//Happy Stick Bat
	"Each chapter of Kid Mystic has a secret "
	"Happy Stick enemy that can only be found "
	"under certain conditions. What are these "
	"conditions? You'll have to play it to find "
	"out! This little guy from chapter 4 spews "
	"out deadly flames so be careful! (SWE)",
	//Tower Guardian
	"Another one of my favorites. His attack pattern is "
	"rather simple: He just jumps to "
	"create shockwaves, and lobs out grenades "
	"around the arena. In Kid Mystic, you had "
	"to fight him four different times in four "
	"different arenas that forced you to fight "
	"him in different ways. (SWE)",
	//Grumpy Golem
	"This angry golem was originally the second "
	"level summon spell in Kid Mystic. It "
	"defaults to being an enemy this time "
	"around though, so keep that in mind. (SWE)",
	//Bobby Khan
	"Who is the mysterious Bobby Khan? You'll "
	"have to play Kid Mystic to find out! "
	"Overall, he's a bit tougher than the "
	"similarly sized Super Duper Zombie. In "
	"Kid Mystic, you get to search for a "
	"powerful weapon and unlock a spell in "
	"order to make his fight much easier, "
	"but this time you'll have to do it "
	"the old fashioned way! (SWE)",
	//Blocko The Great
	"One of the few different minibosses in "
	"Kid Mystic. This guy is only vulnerable "
	"when he sticks his tongue out at you. How "
	"rude! Since he's placed up against a "
	"wall in the original game, none of "
	"his attacks shoot backwards. Keep "
	"this in mind when designing your "
	"encounters. Fun fact: This is the "
	"only monster that doesn't vanish when "
	"you beat him. (SWE)",
	//Blocko Tongue
	"",
	//Optimum Octon
	"This chapter 1 boss originally guarded the "
	"bridge to chapter 2 in Kid Mystic. The "
	"eGames featured a simple battle against a "
	"Mama Spider instead, so I like this much "
	"better. It only fires downward so keep "
	"that in mind. Throw in a couple of Octons "
	"for a truly classic feel. (SWE)",
	//Octentacle
	"DANGER: Do not use unless you know what "
	"you are doing! This is the Optimum Octon's "
	"tentacle! (SWE)",
	//Octentacle
	"DANGER: Do not use unless you know what "
	"you are doing! This is the Optimum Octon's "
	"tentacle! (SWE)",
	//Microfish
	"My personal favorite. These little guys "
	"don't do anything and exist solely for a "
	"gag in the Library level of Kid Mystic. It "
	"took me years to get it, myself. With the "
	"fancy features of Supreme, I'm sure they "
	"could be of some use. (SWE)",
	//Froggy
	"Identical to the ones from Loonyland in "
	"every way. They're a weak early game enemy "
	"in both games that's easy to beat and can "
	"only damage you if you get too close. (SWE)",
	//Toadie
	"They're slightly more dangerous than "
	"Ribbits given that they actually shoot at "
	"you. They aren't as complex as some of the "
	"variants found in Loonyland, but they fill "
	"in that nice \"low level early game enemy\" "
	"spot in an RPG. (SWE)",
	//Mad Cow
	"It's a lot like the Triceroids but in cow "
	"form. It's much easier to deal with, "
	"though. (SWE)",
	//Extremely Mad Cow
	"This cow is REALLY upset! Why does it have "
	"so much HP? In Sleepless, you had to feed "
	"it in order to calm it down, but it's "
	"technically possible to do it the old fashioned way. (SWE)",
	//Flying Pumpkin
	"It was only a matter of time before "
	"Pumpkins learned to fly. It's similar to "
	"the Boomkin except it explodes into "
	"flames. (SWE)",
	//Simulated Head
	"DANGER: Do not use unless you know what "
	"you are doing! This is the simulated head "
	"spawned by the Headless Horseradish. (SWE)",
	//Beetle Nest
	"It's a Beetle Nest! You remember The "
	"Beetles? Well, now they have a nest. (SWE)",
	//Toad Hole
	"Why does a \"Toad Hole\" look like a bush? "
	"That's easy. The hole is under the bush. "
	"Since it's an early game enemy, it doesn't "
	"have as much health as you might expect it "
	"to. (SWE)",
	//Happy Stick Tree
	"What's a Hamumu game without a Happy Stick "
	"enemy? This one's tucked away in a hard to "
	"find spot, so you might beat Sleepless "
	"without ever seeing one. It isn't that "
	"complicated an enemy being a tree and all, "
	"but it's still pretty dang tough! (SWE)",
	//The Darkness
	"Ignore the preview. This creature has no "
	"true form, and is actually the embodiment "
	"of darkness itself! It's not all that "
	"tough, but it's a really cool concept. If "
	"you want to be truly evil, place a bunch "
	"of them in pitch darkness for the player "
	"to discover. (SWE)",
	//Spark
	"A simple puzzle based enemy that cannot "
	"exist unless it's against a wall. In the "
	"Tomb of Sparks, it would travel along them "
	"in order to activate various switches. It "
	"also does contact damge if you aren't "
	"very carefuly. (SWE)",
	//Lightswitch
	"A clever way to light up an area. "
	"Unfortunately, hitting it again doesn't "
	"turn it back off. Imagine the electric "
	"bill from these things. (SWE)",
	//Blaster
	"It resembles the lightballs from "
	"Loonyland, except it shoots at you. The "
	"redder it gets, the closer it is to "
	"firing. Unlike turrets, it aims directly "
	"at you at all times. (SWE)",
	//Charge Coil
	"They're used for various puzzles in the "
	"Tomb of Sparks. If they're placed next "
	"to each other, they'll gain a charge. "
	"If placed near a wall, they'll generate "
	"a Spark. It won't work if there's less "
	"than three of them together. (SWE)",
	//The Grue
	"Unlike the Mama Spider, it doesn't spit "
	"acid at you. It also is much tougher in "
	"darker areas, so make sure to put a bit of "
	"light in if you don't want the player to "
	"be immediately decimated. Think of it like "
	"an inverse stealth level. Also it "
	"periodically spawns Eensy Weensies and "
	"Creepy Crawlies, so watch out for that. (SWE)",
	//Sir Spookley Hauntmore
	"In the original game, you had to hop "
	"between two dimensions in order to beat "
	"it. Since this functionality doesn't "
	"exist in Supreme, it will simply turn "
	"transparent periodically in order to heal. "
	"Because of this, it takes longer to beat "
	"than you would think. (SWE)",
	//Spookley
	"DANGER: Do not use unless you know what "
	"you are doing! This is the transparent "
	"version of Sir Spookley Hauntmore. (SWE)",
	//Speedy Hernandez
	"While he occasionally shoots at you, the "
	"main difficulty behind him is that he's "
	"just too darn difficult to hit! In "
	"Sleepless, you had your trusty "
	"Sparkthrower to hit him with its homing "
	"shots, but this time you'll have to "
	"improvise. (SWE)",
	//Iron Skull
	"This Richie Lich variant has the second "
	"highest HP of any Hamumu enemy ever! It's "
	"basically the same as Richie Lich except "
	"it doesn't poison you and sometimes "
	"summons Boneheads. In the original, you "
	"had to rely on your reflector hammer in "
	"order to beat it, but this time you aren't "
	"so lucky. Though there IS a certain weapon "
	"that might help if you want a more "
	"accurate battle. (SWE)",
	//Big Head Todd
	"This one lobs grenades at you "
	"periodically. They aren't meant to be "
	"super challenging, since you can go into "
	"any temple in Sleepless at any order. I "
	"wonder how their heads got so big? (SWE)",
	//Big Head Maude
	"(pronounced \"mod\") "
	"One of the few minibosses in Sleepless. "
	"The three of them can be found in the Tomb "
	"of Bones. This particular one shoots out "
	"yellow shells at you periodically. (SWE)",
	//Big Head Rod
	"He simply bounces around leaving fire in "
	"his wake, nothing too complex. Fun Fact: "
	"these are the only boss level enemies that "
	"can be summoned via the ledger at the Inn! "
	"(SWE)",
	//Big Head Bod
	"DANGER: Do not use unless you know what "
	"you are doing! This is the body for the "
	"three Big Head bosses. (SWE)",
	//Glass Jaw
	"It's the same as a standard Bonehead in "
	"nearly every way, except it's native to "
	"the Tomb of Reflection. It's also "
	"periodically summoned by the nefarious "
	"Rafe Lector near the end of the game. "
	"As an added bonus to SWE, it'll make a "
	"glass shattering sound when it dies. (SWE)",
	//Rafe Lector
	"A Stan Glass variant that spawns Glass "
	"Jaws periodically. If I recall, it would "
	"also reflect your hammers back at you and "
	"required a special ability to beat. This "
	"time around, simple hammers will do. (SWE)",
	//Lady Von Spatula
	"Unlike Countess, she's no pushover. Even "
	"before she's awake she'll start summoning "
	"her bats (max 8), so take note of that "
	"when you use her. Once she wakes up, "
	"they'll start firing at you periodically "
	"so be prepared! (SWE)",
	//Royal Guard
	"DANGER: Do not use unless you know what "
	"you are doing! This is the guardian "
	"bat for Lady Von Spatula. (SWE)",
	//Scaredy Bat
	"It's so scared of you. I wonder why? "
	"It was featured in one of the tombs where you'd "
	"have to use this to your advantage "
	"in order to guide it to various switches. "
	"(SWE)",
	//Headless Horseradish
	"The final boss of Sleepless Hollow. You're "
	"never actually meant to kill it "
	"originally, hence the stupidly high HP. "
	"Rather you fend off its attacks with all "
	"your acquired abilities. If you want him "
	"to be a regular boss in your world, you'll "
	"probably want to lower its HP a bit. (SWE)",
	//Sparky
	"I've never met a dog named \"Sparky\" in "
	"real life, have you? This electrified "
	"pooch is one of the tougher dog based "
	"enemies in the Hamumuverse. In Sleepless, "
	"it guarded a candle, which is valuable "
	"currency instead of a bonus collectable. (SWE)",
	//Fire Bug
	"Every temple in Sleepless has their own "
	"set of themed enemies. This spitter "
	"variant shoots out red bullets and "
	"occasionally leaves behind a flame. "
	"Nothing too complex. (SWE)",
	//Swampdog
	"One of the more unique species "
	"added to Loonyland. Like their name "
	"suggests, they mainly inhabit the "
	"Slurpy Swamp. Like Thingies, they do a bit "
	"of wandering around on their own when "
	"they're not chasing you directly, so keep "
	"that in mind. (SWE)",
	//Bog Beast
	"A more aggressive Swampdog variant with "
	"higher HP. Did you notice that Swampdogs "
	"look a lot like Thingies? One popular "
	"theory is that they're the same species, "
	"and they don't grow a second eye until "
	"later. Isn't life interesting? (SWE)",
	//Shambling Thing
	"The toughest Swampdog variant prior "
	"to the 2.0 update. It fires a spread of "
	"red bullets at you sequentially rather "
	"than all at once. (SWE)",
	//Marsh Mutt
	"This dangerous dog usurped the Shambling "
	"Thing as the most dangerous Swampdog breed "
	"post 2.0. He has a lot of HP and shoots "
	"out deadly triple-bullets instead of "
	"normal ones. (SWE)",
	//Bonehead
	"Basically the same as a normal Bonehead "
	"but with a larger head. It made sense to "
	"port these over since the Bonehead-O- "
	"Matics use their model. I asked Jamul why "
	"he gave them bigger heads and he only "
	"had this to say: "
	"\"Yes, bigger is cuter. I was refining my style, "
	"and I'd do it again and bigger too, "
	"if it weren't for you meddling kids.\" "
	"(SWE)",
	//Redbone
	"Stronger than a Bonehead and slightly more "
	"aggressive. If you've been around the "
	"Hamumu community long enough, you might "
	"be more familiar with this particular "
	"enemy. (SWE)",
	//Gangrene
	"A sickly Bonehead variant that spews out "
	"poison. Since health and poison work "
	"differently in Supreme than they do in "
	"Loonyland, you might find yourself having "
	"dealing with the poison based enemies a "
	"bit differently. (SWE)",
	//Numbskull
	"Not the strongest Bonehead variant, "
	"but his ability to resurrect any nearby "
	"Bonehead variants make him the most "
	"dangerous. Definitely target him first! "
	"(SWE)",
	//Agent Skully
	"Agent Skully. Get it? It's not a true "
	"Hamumu game unless there are obscure "
	"references thrown in everywhere. He's "
	"the most powerful out of all Bonehead "
	"variants out there. So powerful in fact "
	"that the Numbskull can't revive him. What "
	"a relief! (SWE)",
	//Croaker
	"Probably the most dangerous of the frog "
	"species. When it dies, it releases clouds "
	"of poison everywhere, so don't get too "
	"close! (SWE)",
	//Frogurt
	"A frog variant that spawns out of a "
	"Torture Tree. It might be extremely weak, "
	"but the constant trail of flames it spawns "
	"linger for a bit after it's killed. The "
	"more room you have to dodge around them "
	"the less dangerous they are. (SWE)",
	//Toad
	"A weak frog that shoots at you and spawns "
	"a single flame when it dies. Not too "
	"deadly but can still cause a bit of "
	"damage if you aren't paying attention. "
	"(SWE)",
	//Zombie
	"Prior to his obsession with Zombies, they "
	"were just another enemy for Loony to beat. "
	"The Loonyland version moves slightly "
	"different and has a bigger head but is "
	"largely the same. None of the Loonyland "
	"Zombies will drop brains, either. (SWE)",
	//Bombie
	"This is a pun that practically writes "
	"itself. They might be weak, but don't let "
	"them get too close to you. Their attacks "
	"can be rather... explosive. (SWE)",
	//Zombie Lord
	"One of the most dangerous Zombie types. It "
	"has a powerful kicking attack which spawns "
	"semi-homing dirt spikes. In Loonyland, "
	"Loony has to clear three crypts of their "
	"Zombie Lords in order to free Zombiton "
	"from a horrible curse. I wonder if this "
	"endeavor had a lasting effect on him? "
	"(SWE)",
	//Bombie Lord
	"Upon the release of the Loonyland 2.0 "
	"patch came several new enemy types, "
	"characters, and even a new mode. One such "
	"addition was the Bombie Lord, which "
	"periodically summons Bombies instead of "
	"ground spikes. He's actually more "
	"dangerous than the Zombie Lord. (SWE)",
	//Dingbat
	"Not much to say, really. Slightly "
	"different from your regular Scary Bat but "
	"functionally the same. "
	"You might wonder why some entries have "
	"(SWE) at the end. It simply means "
	"that this monster was added in with "
	"the \"Supreme with Everything\" "
	"edition, and as such the commentary "
	"was written by someone other than "
	"Jamul, himself. (SWE)",
	//Gargoyle
	"The hardiest of all of the bat variants of "
	"Loonyland. It makes sense given that it's "
	"made out of stone. (SWE)",
	//Vamp Bat
	"Unlike the other Loonyland bat variants, "
	"this guy can shoot at you. It's mostly "
	"found in Castle Vampy. (SWE)",
	//Gasbat
	"Weaker than a Gargoyle, but far deadlier. "
	"These nasty buggers spread poison wherever "
	"they go. Very dangerous in closed spaces. "
	"(SWE)",
	//Rag Man
	"The weakest Mummy variant. They only use "
	"melee attacks, but they're still faster "
	"than the Mumbles of Supreme. ",
	//Mummy
	"A Stronger Rag Man that fires bullets. I "
	"guess that's what the gems are for. Why "
	"do Ragmen have them too? I guess they "
	"simply aren't strong enough to use them. "
	"(SWE)",
	//Pharaoh Faucet
	"A Strong Mummy variant that shoots out "
	"a burst of bullets. In Loonyland, the "
	"red bullets were a bit larger and thus "
	"slightly more imposing. Since they're "
	"exactly the same as the red bullets in "
	"Supreme, they weren't ported over. (SWE)",
	//Grand Pharaoh
	"Prior to the 2.0 patch this was the "
	"deadliest Mummy variant that you can find. "
	"He can summon two Ragman minions at a time "
	"and shoot a triple spread of bullets at "
	"you as well. (SWE)",
	//King Toot
	"With a name like \"King Toot\" you'd expect "
	"him to shoot poison but he doesn't. He "
	"DOES summon out Pharoh Faucets instead of "
	"Ragmen so be careful of that. (SWE)",
	//Terror Tree
	"This evil tree spawns Toads from its "
	"mouth. It may LOOK scary, but that's "
	"about the only thing it does. In "
	"Loonyland, one of the first quests you "
	"get is to rid a small forest of their "
	"presence. It's a shame you don't really "
	"see them anywhere else. (SWE)",
	//Swamp Stump
	"A Terror Tree variant that regularly "
	"spews out poison. A great obstacle for "
	"narrow hallways. It can't be damaged "
	"directly though, which is unfortunate "
	"because you'll never get to see its name "
	"in the game. In Loonyland, the only "
	"way around this was to acquire the "
	"mythical \"touch of death\" badge, which "
	"lets you kill literally anything in one "
	"hit. (SWE)",
	//Torture Tree
	"A Terror Tree variant from the 2.0 patch "
	"that spawns Frogurts instead of Toads. "
	"Despite being alot more durable, this is "
	"still its only method of attack. (SWE)",
	//Ghost
	"Unlike the Spooky Ghosts of Supreme, these "
	"guys prefer to get close and punch you. "
	"They also don't teleport away when you hit "
	"them. Don't let too much of them surround "
	"you because the damage adds up fast! (SWE)",
	//Shade
	"A hardier Ghost that cloaks itself in "
	"a mantle of darkness, which not many "
	"Hamumu based enemies do. Other then that, "
	"it's pretty much the same as a standard "
	"Ghost. (SWE)",
	//Banshee
	"Watch out for their screams. They're so "
	"scary they can freeze you in their tracks! "
	"If there's enough of them around, you just "
	"might be frozen forever. (SWE)",
	//Dogboy
	"The original version of the Weremutt. "
	"Identical in every way apart from its "
	"name. In Loonyland, you had to complete "
	"a special quest in order to harm their "
	"kind. Everyone knows you need a silver "
	"bullet to hurt werewolves! (SWE)",
	//Wolfman
	"A stronger werewolf variant that shoots "
	"at you. An easy recipe for variants is to "
	"give it more HP and have at shoot at you. "
	"Nothing fancy about it. (SWE)",
	//Dire Wolf
	"Prior to the 2.0 patch, this was the "
	"strongest werewolf enemy you could find. "
	"Be careful when it howls, it can summon "
	"Dogboys from offscreen. (SWE)",
	//Happy Stick Witch
	"One of the most powerful Happy Stick "
	"enemies in the Hamumuverse. So powerful in "
	"fact that she predates Count Bonkula's "
	"Evilizer scheme entirely. Just who is "
	"she? What is she planning? Perhaps the "
	"two Loonyland games could shed more light "
	"on her nefarious schemes. (SWE)",
	//Larry
	"He became a vicious werewolf and hid in "
	"a werewolf den just to hide from his wife! "
	"Not only that, but he kept the Pumpkin key "
	"all to himself! Loony was sent on a quest "
	"to defeat this selfish man and retrieve "
	"the key to continue his quest. This time "
	"around won't be any easier since he's "
	"immune to your attacks. (SWE)",
	//Human Larry
	"",
	//Harry
	"Similar to Larry, except he can take "
	"damage. He's part of the elite class of "
	"enemies added to 2.0 shared by the likes "
	"of the Bombie Lord and Pharoh Faucet. You "
	"know it's tough when they spew out wispy "
	"skulls when they die! (SWE)",
	//Bonehead-O-Matic
	"These machines generate Boneheads "
	"endlessly. You have to wonder where it "
	"finds all of those bones. In Loonyland, "
	"you have to find and destroy all of them "
	"in order to venture deeper into Mystery "
	"Lake. (SWE)",
	//Redbone-O-Matic
	"It's an \"O-Matic\" variant that cranks "
	"out Redbones. It along with the Gangrene "
	"and Numbskull variants were added in the "
	"2.0 patch long ago. (SWE)",
	//Gangrene-O-Matic
	"Come 2.0, brand new \"O-Matics\" were made "
	"for Redbones, Gangrenes, and Numbskulls "
	"alike. Each of them having more HP than a "
	"standard Bonehead-O-Matic. Like most of "
	"the enemies added in 2.0, they can't be "
	"found in the standard Adventure mode map. "
	"(SWE)",
	//Numbskull-O-Matic
	"There's nothing more annoying than a "
	"machine that generates badguys that "
	"keep other badguys from dying. Like the "
	"Numbskulls it generates, you should "
	"try to kill it first if you plan to kill "
	"any of the other Boneheads around. (SWE)",
	//Vamp Jr.
	"Despite looking like an SWC vampire with "
	"a larger head, this is actually the "
	"weakest type of vampire. (SWE)",
	//Bloodsucker
	"Weirdly enough he prefers to shoot at you "
	"from afar rather than get up close. Kind "
	"of a counter intuitive name, really. "
	"He shares the same color pallete as Spikey, but he's "
	"far less dangerous. (SWE)",
	//Draculite
	"He shoots out poison, and can summon "
	"Gangrenes which also shoot poison. Not to "
	"mention that he's completely green so he "
	"might just be made of poison. (SWE)",
	//Vamp Lord
	"The hardiest of all the normal vampires. "
	"He might not attack you himself, but he "
	"can still overwhelm you by constantly "
	"summoning waves of bats and ghosts at you. "
	"(SWE)",
	//Frankenjulie
	"This giant mechanical gal was built in "
	"order to guard the Skull Key at the bottom "
	"of Mystery Lake. She has three phases "
	"total with differing attack patterns for "
	"each. She also deals contact damage so be "
	"careful! (SWE)",
	//Frankenhand
	"DANGER: Do not use unless you know what "
	"you are doing! This is one of Frankenjulie's hands. (SWE)",
	//Mini-Frankenjulie
	"A miniature version of Frankenjulie "
	"built to guard a powerup hidden at the "
	"center of an artpiece. She's not quite "
	"as dangerous as Frankenjulie but still "
	"no pushover. (SWE)",
	//Count Bonkula
	"The main antagonist of Loonyland! I guess "
	"Bouapha isn't the only one who wields an "
	"oversized hammer. His scheme to plunge the "
	"world into darkness with the Evilizer was "
	"eventually thwarted by none other than "
	"a young Dr Lunatic. (SWE)",
	//Mecha-Bonkula
	"A Mechanized version of Count Bonkula "
	"exclusive to 2.0. Unlike his counterpart, "
	"he can be damaged with normal means. (SWE)",
	//The Evilizer
	"This evil machine was created by Count "
	"Bonkula in order to spread darkness and "
	"evil throughout the world. Even though "
	"Count Bonkula was defeated, Loony still "
	"had a tough battle against the machine "
	"itself. I like the concept of a scheme "
	"that outlives the villain that made it. "
	"Note that during its second phase it will "
	"spawn various powerful monsters from "
	"Loonyland offscreen, so keep that in mind. "
	"(SWE)",
	//Steam Pump
	"A powerful turret with a skull for a head. "
	"In Loonyland, it pumps steam into the "
	"Evilizer in order to power it. Watch out "
	"for the gas. It's surprisingly hard to "
	"see! Note that you have to destroy all of "
	"these on the map in order to damage the "
	"Evilizer. Not 100% the same as the "
	"original fight but it works well enough. "
	"(SWE)",
	//Gusty The Elder
	"One of the four Elder Vampires of Castle "
	"Vampy. He's spent hundreds of years honing "
	"himself in order to become impervious "
	"to slingshot fire. Unfortunately, that isn't "
	"helpful to him in this game. Perhaps "
	"his ghostly minions and wind projectiles "
	"can still make up for it? (SWE)",
	//Stoney The Elder
	"One of the four Elder Vampires of Castle "
	"Vampy. In the original game, he was "
	"totally invincible! You had to slowly push "
	"him off the top of the tower, hence why he "
	"doesn't move. This time around, you can "
	"damage him the normal way. (SWE)",
	//Drippy The Elder
	"One of the four Elder Vampires that rule "
	"Castle Vampy. This one has mastery over "
	"water and ice, and is the only one of "
	"the four who doesn't deal damage directly. "
	"Make sure to take advantage of his "
	"annoying tendancies properly when you "
	"design a level for him. He'll need a "
	"little help in order to win. (SWE)",
	//Toasty The Elder
	"One of the four Elder Vampires of Castle "
	"Vampy. He's mastered his powers over fire "
	"and is so hot that even Loony's slingshot "
	"couldn't damage him. Fortunately, it seems "
	"like he can take damage this time "
	"around. (SWE)",
	//Summony The Elder
	"He's actually NOT one of the four Elders, "
	"and was added in 2.0. I guess the other "
	"four didn't want to bother building an "
	"extra tower. He's got the most life out of "
	"all of them, but he never attacks you "
	"directly, instead relying on the minions "
	"he constantly summons to do his dirty "
	"work. (SWE)",
	//Bubble
	"DANGER: Do not use unless you know what "
	"you are doing! This is the bubble spawned "
	"by Drippy The Elder. (SWE)",
	//Villager M
	"Supreme doesn't have the dialogue system "
	"that Loonyland does, but it still made "
	"sense to port these over. It'll be nice "
	"to be able to use normal humans in urban "
	"themed levels for a change. Did you "
	"notice that their heads are the same as "
	"a Wacko's?(SWE)",
	//Villager M
	"Another villager variant. Since they're "
	"completely invincible, you couldn't "
	"attack them at all in Loonyland, but with "
	"a certain badge, you could find your way "
	"around this. (SWE)",
	//Villager M
	"This variant is the same model Harry Larry "
	"uses. Also worthy of note is a villager "
	"named Kinyova who teaches you about "
	"special weapons. You can meet his father "
	"in Loonyland 2. (SWE)",
	//Villager F
	"One villager that used this model "
	"was Zizwalda the witch, who sent you "
	"on various quests in order to concoct "
	"a ghost hunting potion. Prior to this, "
	"your shots would just pass right through! "
	"Be careful not to bug her too often or "
	"she'll turn to into a frog for the rest "
	"of the game! (SWE)",
	//Villager F
	"You have to have both male and female "
	"villagers to balance things out. It just "
	"makes sense that way. One such villager "
	"sent you on your way to find a lost cat "
	"of hers. It was surprisingly hard to "
	"find! (SWE)",
	//Villager F
	"The third female villager variant. One "
	"such villager warned Loony of the "
	"impending doom of Halloween Hill and sent "
	"him on his main quest. (SWE)",
	//PolterGuy
	"One of the main bosses in Loonyland. He "
	"guards the Bat Key at the top of "
	"an ancient tower filled with ghosts. Be "
	"sure to utilize the Junk he interacts with "
	"somehow. He's not very tough without them. "
	"(SWE)",
	//Junk Barrel
	"DANGER: Do not use unless you know what "
	"you are doing! This is the Junk weilded by "
	"Polterguy. Included in SWE are several items "
	"that resemble Junk. They're hardcoded to "
	"gradually turn into Junk if a Polterguy "
	"is present. (SWE)",
	//Junk Chair
	"DANGER: Do not use unless you know what "
	"you are doing! This is the Junk weilded by "
	"Polterguy. Included in SWE are several items "
	"that resemble Junk. They're hardcoded to "
	"gradually turn into Junk if a Polterguy "
	"is present. (SWE)",
	//Junk Table
	"DANGER: Do not use unless you know what "
	"you are doing! This is the Junk weilded by "
	"Polterguy. Included in SWE are several items "
	"that resemble Junk. They're hardcoded to "
	"gradually turn into Junk if a Polterguy "
	"is present. (SWE)",
	//Ghastly Stench
	"One of four bosses added in 2.0. He "
	"spreads toxic gas everywhere he floats and "
	"even summons Gasbats. He's also the only "
	"boss in Loonyland that can be found in "
	"more than one place. (SWE)",
	//Firebat
	"A deadlier version of a standard Scary Bat "
	"that leaves a trail of fire wherever it "
	"goes. It's harder to kill, too. Now THAT "
	"is scary! (SWE)",
	//Laser R
	"A big part of these guys were the mirror "
	"based reflecting puzzles, which "
	"unfortunately couldn't be ported over. "
	"If you want to see these guys in their "
	"full glory, you'll have to visit the "
	"Tomb of Reflection in Sleepless Hollow. "
	"(SWE)",
	//Laser D
	"Originally, it could be only defeated by "
	"its own laser, which is a death sentence "
	"even at max health! Use these for some "
	"interesting puzzles or impassible "
	"barriers. (SWE)",
	//Laser L
	"These lasers never change direction on "
	"their own, so there are four versions "
	"of them. Utilize the \"change monster\" "
	"special to have them switch up on "
	"unsuspecting players! (SWE)",
	//Laser U
	"It's a Laser turret that fires upward. I "
	"ran out of interesting things to say about "
	"them, so here's a factoid: the Evil Clone "
	"makes an appearance in Tomb of Mirrors "
	"under the name \"Your Bad Side\". (SWE)",
	//Megaburner
	"It's a burner that fires three blasts of "
	"flame instead of one! It only fires on the "
	"sides, and it's proximity based, so make "
	"a note of that. (SWE)",
	//Peeping Bomb (CCW)
	"Same as Peeping Bomb, but spins "
	"counterclockwise. (SWE)",
	//Kid Mystic
	"DANGER: Do not use unless you know what you are doing! "
	"This is the dummy monster for the playable Kid Mystic. (SWE)",
	//Kid Mystic (Armageddon Sword)
	"DANGER: Do not use unless you know what you are doing! "
	"This is the dummy monster for the playable Kid Mystic with "
	"Armageddon Sword. (SWE)",
	//Creepy Crawly
	"Almost identical to their Spitter cousins, "
	"except they're a bit faster. In Sleepless "
	"Hollow, they call the final dungeon their "
	"home, and are the one of the main minions "
	"of the evil Grue. (SWE)",
	//Deadhead
	"These immortal denizens of the Tomb of "
	"Darkness can NEVER be killed! Use these "
	"guys when a few Crazybones simply won't "
	"cut it. Be careful when designing a level "
	"around them, though. (SWE)",
	//Vulnerable Evilizer
	"",
	//Dumble
	"These Thingie variants lived in an "
	"alternate dimension known as \"The Dumb "
	"Side\", which can only be accessed with the "
	"Planetsmasher hammer. While they have "
	"more HP, they're the same as a Thingie in "
	"every other aspect. (SWE)",
	//Marsh Marrow
	"Not to be confused with the similarly "
	"colored Gangrene. Instead of poison gas, "
	"these guys shoot out three green bullets. "
	"In Sleepless, these guys would spawn in "
	"the later half of the woods that guarded "
	"the final cave. (SWE)",
	//Friendly (Bald)
	"There were only a few different NPCs in "
	"Kid Mystic. They just stood still waiting "
	"for you to talk to them and tell you "
	"something. Much less complex than "
	"Loonyland. This particular version was "
	"unused. Though he DOES look a little "
	"familiar. (SWE)",
	//Friendly (Blonde)
	"I love the hastily scribbled on hair. He's "
	"completely unrecognizable, don't you "
	"think? This guy was waiting for you at the "
	"start of chapter one and told you how to "
	"play. (SWE)",
	//Friendly (Girl)
	"Only the most dedicated Hamumu fans know "
	"what game this girl is originally "
	"from. It's a shame she isn't as popular as "
	"her older brother is. Maybe she just "
	"doesn't hate pumpkins as much as him. "
	"(SWE)",
	//Friendly (Wizard)
	"Who is this mysterious wizard and what "
	"does he want? You'll have to play Kid "
	"Mystic to find out! These four entities "
	"won't actually *DO* anything on their own "
	"in this game, but it's nice to finally "
	"have something besides Pygmies to put in "
	"urban levels. (SWE)",
	//Smashball
	"One particular level of Kid Mystic had you "
	"bouncing around a ball in order to break "
	"bricks, Arkanoid style! It was kind of "
	"clunky and I don't even remember how to "
	"get there, but we couldn't leave out such "
	"a novel idea so we brought it over here. "
	"What sort of wacky things can YOU do with "
	"it? (SWE)",
	//Happy Stick Dancer
	"From the Super Ultra Mega Secret Reward "
	"Level hidden in Kid Mystic. In the "
	"original, they dropped a ton of cash so "
	"you could buy a bunch of stuff in the next "
	"loop, but this time around it looks like "
	"they're just here to have a good time. (SWE)",
	//Lightball2.5
	"These types of Lightballs switch on and "
	"off at a specific rate. Very usefull for "
	"when you want to have the player slowly "
	"make their way through a gauntlet or "
	"something. See Loonyland's Mystery Lake "
	"for a good reference. (SWE)",
	//Lightball5.2
	"This version switches on and off at a "
	"slower rate. It's not much of a gauntlet "
	"if the obstacles behave the same way! By "
	"the way, Lightballs will only activate "
	"if a foe is nearby. It probably saves "
	"on their electric bill that way. (SWE)",
	//Lightball
	"Lightballs were a unique addition to "
	"Loonyland. Alone, they are useless, but "
	"when you connect two or more, they form a "
	"barrier of lightning between them! They're "
	"actually slightly more dangerous in this "
	"game though, as you can't rely on "
	"invincibility frames to protect you. This "
	"is the only Lightball variant in Loonyland "
	"that can take damage. (SWE)",
	//LightballOn
	"The strongest Lightball variant. It's "
	"ALWAYS on, hence the name. If you want "
	"this version to turn off, you'll have to "
	"utilize specials for that. (SWE)",
	//LightballOff
	"It's a Lightball that's been turned off. (SWE)",
	//Starey bat (strong)
	"(SWE)",
	//vulnerable Lookey-Loo
	"",
};

char *MonsterNotes(dword type)
{
	return monsNotes[type];
}

