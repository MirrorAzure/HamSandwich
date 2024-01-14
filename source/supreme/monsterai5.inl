void SelectDestination(Guy *me,Guy *goodguy,Map *map,byte repeat)
{
	int i,x,y;
	byte bestx[5],besty[5];

	for(i=0;i<5;i++)
		bestx[i]=255;

	for(x=0;x<map->width;x++)
		for(y=0;y<map->height;y++)
		{
			if((x!=me->mind2 || y!=me->mind3 || repeat==1))//map->map[x+y*map->width].tag==1 && 
			{
				// there is a tag here... which is NOT my previous target
				// if(map->CheckLOS(me->mapx,me->mapy,7,x,y))
				// {
					// and it's in line of sight
					for(i=0;i<5;i++)
					{
						if(bestx[i]==255)
						{
							bestx[i]=x;
							besty[i]=y;
							i=5+2;
						}
					}
					if(i!=5+3)
					{
						// all 3 tags are presently occupied, so let's see
						// if this is closer than one of them
						for(i=0;i<5;i++)
						{
							if(abs(bestx[i]-me->mapx)+abs(besty[i]-me->mapy)>
								abs(x-me->mapx)+abs(y-me->mapy))
							{
								bestx[i]=x;
								besty[i]=y;
								i=5+2;
							}
						}
					}
					// now if the tag is one of the closest 3 visible ones, then
					// it is in the best list
				//}
			}
		}
	// now we have a list of up to 3 tags within sight
	// which one is closest or farthest from Loony?
	if(me->type==MONS_LARRY)
	{
		// want to kill - go TOWARDS loony
		x=255;
		for(i=0;i<5;i++)
		{
			if(x==255 ||
				(bestx[i]!=255 && abs(bestx[i]-goodguy->mapx)+abs(besty[i]-goodguy->mapy)<
						abs(bestx[x]-goodguy->mapx)+abs(besty[x]-goodguy->mapy)))
				x=i;
		}
		if(x==255)
		{
			// no visible valid tags, so head straight for Loony!
			me->mind2=goodguy->mapx;
			me->mind3=goodguy->mapy;
		}
		else
		{
			me->mind2=bestx[x];
			me->mind3=besty[x];
		}
	}
	else
	{
		// want to run - go AWAY from loony
		x=255;
		for(i=0;i<5;i++)
		{
			if(x==255 ||
				(bestx[i]!=255 && abs(bestx[i]-goodguy->mapx)+abs(besty[i]-goodguy->mapy)>
						abs(bestx[x]-goodguy->mapx)+abs(besty[x]-goodguy->mapy)))
				x=i;
		}
		if(x==255)
		{
			// no visible valid tags, so flee randomly
			me->mind2=(byte)Random(map->width);
			me->mind3=(byte)Random(map->height);
		}
		else
		{
			me->mind2=bestx[x];
			me->mind3=besty[x];
		}
	}
}

void BasicAI(Guy *me,int ouchSound,int dieSound,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(ouchSound,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(dieSound,me->x,me->y,SND_CUTOFF,1200);
	}
}

void DoMove(Guy *me,int move,int frmAdv,byte busy,int dx,int dy)
{
	me->seq=move;
	me->frm=0;
	me->frmTimer=0;
	me->frmAdvance=frmAdv;
	if(busy)
		me->action=ACTION_BUSY;
	else
		me->action=ACTION_IDLE;
	me->dx=dx;
	me->dy=dy;
}

void AI_Ptero(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	Guy *g;

	g=GetGuyOfAIType(MONS_BOUAPHA);

	if(g==NULL)
	{
		// vanish if there is no goodguy to follow
		me->type=MONS_NONE;
		return;
	}

	if(me->reload)
		me->reload--;

	if(me->mind1)
		me->mind1--;

	if(me->ouch==4)
		MakeSound(SND_PTERODIE,me->x,me->y,SND_CUTOFF,1200);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0)
		{
			// shoot
			FaceGoodguy(me,goodguy);
			MakeSound(SND_PTEROSHOOT,me->x,me->y,SND_CUTOFF,600);
			FireBullet(me->x,me->y,me->facing*32,BLT_ENERGY,me->friendly);
			me->reload=25;
		}
		if(me->seq==ANIM_ATTACK)
		{
			Dampen(&me->dx,FIXAMT*2);
			Dampen(&me->dy,FIXAMT*2);
		}
		if(me->seq==ANIM_DIE)
		{
			BlowSmoke(me->x,me->y,me->z,0);
		}
		return;
	}
	if(me->mind==0)	// standard combat mode
	{
		if(goodguy==NULL || goodguy->type==MONS_FRIENDLY || RangeToTarget(me,g)>200*FIXAMT)
		{
			// too far from goodguy, run home
			me->mind=1;
		}
		else
		{
			if(RangeToTarget(me,goodguy)>300*FIXAMT)	// too far to fight, go home
			{
				me->mind=1;
			}
			else
			{
				if(me->mind1==0)
				{
					FaceGoodguy2(me,goodguy);
					me->mind1=2;
				}
				if(me->reload==0 && RangeToTarget(me,goodguy)<128*FIXAMT)
				{
					// don't have to stop to shoot
					MakeSound(SND_PTEROSHOOT,me->x,me->y,SND_CUTOFF,600);
					FireBullet(me->x,me->y,me->facing*32,BLT_ENERGY,me->friendly);
					me->reload=10;//-SpellLevel()/5+4;
				}
			}
		}
	}
	else	// running home mode
	{
		if(RangeToTarget(me,g)<128*FIXAMT && goodguy && RangeToTarget(me,goodguy)<300*FIXAMT)
		{
			// got close enough, get violent again
			me->mind=0;
		}
		if(me->mind1==0)
		{
			FaceGoodguy2(me,g);
			me->mind1=2;
		}
	}
	me->frmAdvance=256;
	me->dx+=Cosine(me->facing*32);
	me->dy+=Sine(me->facing*32);
	Clamp(&me->dx,FIXAMT*10);
	Clamp(&me->dy,FIXAMT*10);
}
	
	
	
void AI_Eyeguy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_EYEGUYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_EYEGUYDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*20;
			y=me->y+Sine(me->facing*32)*20;
			// fire a shot
			FireBullet(me->x,me->y,me->facing,BLT_YELWAVE,me->friendly);
			me->reload=10;
		}
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<(256*FIXAMT) && MGL_random(32)==0 && me->reload==0)
		{
			// spit at him
			MakeSound(SND_EYEGUYSHT,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			FaceGoodguy(me,goodguy);
			return;
		}
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*3;
			me->dy=Sine(me->facing*32)*3;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(MGL_random(64)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && MGL_random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)MGL_random(8);
			me->mind1=MGL_random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*3;
		me->dy=Sine(me->facing*32)*3;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}



void AI_Octopus(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(!goodguy)
		{return;}
		

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_OCTONOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_OCTONDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			//WaterRipple(me->x/FIXAMT,me->y/FIXAMT,10*32);
			if((me->frm==4 || me->frm==10 || me->frm==16) && me->reload==0 && goodguy)
			{
				// spit stuff
				byte f;

				FaceGoodguy2(me,goodguy);
				x=me->x+Cosine(me->facing*32)*14;
				y=me->y+Sine(me->facing*32)*14;
				f=me->facing*32-12+MGL_random(25);
				FireBullet(me->x,me->y,me->facing*32-12+MGL_random(25),BLT_ENERGY,me->friendly);
				me->reload=2;
				MakeSound(SND_OCTONSHOOT,me->x,me->y,SND_CUTOFF,1200);
			}
			if(me->frm==20)
			{
				//WaterRipple(me->x/FIXAMT,me->y/FIXAMT,32*32);
				me->seq=ANIM_A2;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
		}
		if(me->seq==ANIM_A2)
		{
			if(me->frm==5)
				me->mind=0;
		}
		return;	// can't do nothin' right now
	}

	//WaterRipple((me->x+me->dx*2)/FIXAMT,(me->y+me->dy*2)/FIXAMT,8*MGL_random(32));

	if(me->mind==0)	// lurking
	{
		if(RangeToTarget(me,goodguy)<(300*FIXAMT) && MGL_random(48)==0 && me->reload==0)
		{
			ExplodeParticles(PART_WATER,me->x,me->y,0,10);
			//WaterRipple(me->x/FIXAMT,me->y/FIXAMT,64*32);
			FaceGoodguy2(me,goodguy);
			// arise
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			me->mind=1;
			me->mind1=0;
			MakeSound(SND_OCTONSPLASH,me->x,me->y,SND_CUTOFF,1200);
			return;
		}

		if(me->mind1)
			me->mind1--;
		if(!me->mind1)	// time to get a new direction
		{
			me->facing=(byte)MGL_random(8);
			me->mind1=MGL_random(60)+1;
		}
		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
		if(me->seq==ANIM_MOVE && me->frm>=5 && me->frm<=7)
		{
			me->type=MONS_OCTOPUS2;
		}
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
		}
		me->frmAdvance=MGL_random(128)+2;
	}
	else if(me->mind==1)	// arising
	{
		if(me->frm>=5)
		{
			me->type=MONS_OCTOPUS;
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=200;
			me->mind=1;
			me->action=ACTION_BUSY;
		}
	}
	//if((world->terrain[map->map[me->mapx+me->mapy*map->width].floor].flags&(TF_WATER|TF_LAVA))==0)
		//me->GetShot(0,0,20,map,world);
}



void AI_Incabot(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_INCAHIT,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_INCADIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		me->x+=Cosine(me->facing*32)*15;
		me->y+=Sine(me->facing*32)*15;
		if(RangeToTarget(me,goodguy)<(35*FIXAMT) && me->frmTimer<128 && me->reload==0 &&
			map->FindGuy(me->mapx,me->mapy,5,goodguy))
		{
			// get him!
			goodguy->GetShot(me->dx/2,me->dy/2,1,map,world);
			me->reload=4;
		}
		me->x-=Cosine(me->facing*32)*15;
		me->y-=Sine(me->facing*32)*15;
		FaceGoodguy(me,goodguy);

		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}



void AI_Incagen(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	Guy *g;
	int rng,x,y;
	byte f;
	
	if(me->mindControl)
		f=2;
	else
		f=me->friendly;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_INCAHIT,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_INCADIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_MOVE && me->frm==8 && me->reload==0)
		{
			g=AddBaby(me->x,me->y,0,MONS_INCABOT,me);
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				RemoveGuy(g);
			me->reload=30;
			me->mind1=8;
		}
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		return;
	}
	rng=400*FIXAMT;
	
	if(!me->reload && me->mind==1 && goodguy && RangeToTarget(me,goodguy)<rng)
	{
		MakeSound(SND_INCAGEN,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		map->map[me->mapx+me->mapy*map->width].templight=8;
		if(me->mapx>0)
			map->map[me->mapx-1+me->mapy*map->width].templight=0;
		if(me->mapx<map->width-1)
			map->map[me->mapx+1+me->mapy*map->width].templight=0;
		if(me->mapy>0)
			map->map[me->mapx+(me->mapy-1)*map->width].templight=0;
		if(me->mapy<map->height-1)
			map->map[me->mapx+(me->mapy+1)*map->width].templight=0;
	}
	if(goodguy && RangeToTarget(me,goodguy)<rng)
		me->mind=1;
}



void AI_Trapazoid(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if (me->type==MONS_TRAPAZOID && me->seq!=ANIM_ATTACK && me->action!=ACTION_BUSY)
	{
		me->type=MONS_TRAPAZOID2;
	}
	
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SERPENTOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SERPENTDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(goodguy && ((RangeToTarget(me,goodguy))<(150*FIXAMT)) && (me->reload==0) && me->seq!=ANIM_ATTACK)
		{
			FaceGoodguy(me,goodguy);
			me->type=MONS_TRAPAZOID;
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->mind1=0;
			me->reload=90;
			return;
		}
	else
	{
		if(me->action==ACTION_BUSY)
		{
			if(me->seq==ANIM_DIE && me->frm==6)
			{
				me->type=MONS_TRAPAZOID2;
				me->seq=ANIM_IDLE;
				me->frm=0;
				me->frmTimer=0;
			}
			if(me->seq==ANIM_ATTACK)
			{
				if(me->frm>=6 && me->frm<=8)
				{
					FireBullet(me->x,me->y,me->facing,BLT_FLAME2,me->friendly);
				}
				if(me->frm==9)
				{
					me->frm=6;
					me->facing=(me->facing+1)&7;
					me->mind1++;
					if(me->mind1==8)
					{
						me->seq=ANIM_DIE;
						me->frm=0;
						me->frmTimer=0;
						me->frmAdvance=128;
					}
				}
			}

			return;
		}
	}
}



void AI_Incagold(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->action==ACTION_BUSY)
	{
		return;
	}

	if(me->type==MONS_INCAGOLD && me->mind==0)
	{
		me->mind=1;
		if(me->mapx>0 && map->map[me->mapx-1+me->mapy*map->width].wall>0)
		{
			me->facing=0;	// head right
			me->dx=FIXAMT*3/2;
			me->dy=0;
		}
		else
		{
			me->facing=4;	// head left
			me->dx=-FIXAMT*3/2;
			me->dy=0;
		}
	}
	else if(me->type==MONS_INCAGOLD2 && me->mind==0)
	{
		me->mind=1;
		if(me->mapy>0 && map->map[me->mapx+(me->mapy-1)*map->width].wall>0)
		{
			me->facing=6;	// head down
			me->dx=0;
			me->dy=FIXAMT*3/2;
		}
		else
		{
			me->facing=6;	// head up
			me->dx=0;
			me->dy=-FIXAMT*3/2;
		}
	}

	if(me->mind1)	// bumped a wall
	{
		me->mind1=0;

		if(me->type==MONS_INCAGOLD)
		{
			me->facing=4-me->facing;
			me->dx=-me->dx;
		}
		else
		{
			me->facing=8-me->facing;
			me->dy=-me->dy;
		}
	}

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}

	if(goodguy)
	{
		char sway[]={0,1,2,3,4,3,2,1,0,-1,-2,-3,-4,-3,-2,-1};

		if(me->facing==0 && goodguy->x>me->x && abs(goodguy->y-me->y)<32*FIXAMT)
		{
			FireBullet(me->x+FIXAMT*32,me->y-4*FIXAMT-sway[me->frm]*FIXAMT,me->facing,BLT_DEATHBEAM,me->friendly);
			FireBullet(me->x+FIXAMT*32,me->y+4*FIXAMT-sway[me->frm]*FIXAMT,me->facing,BLT_DEATHBEAM,me->friendly);
			if(me->reload==0)
			{
				MakeSound(SND_INCABEAM,me->x,me->y,SND_CUTOFF,100);
				me->reload=10;
			}
		}
		if(me->facing==4 && goodguy->x<me->x && abs(goodguy->y-me->y)<32*FIXAMT)
		{
			FireBullet(me->x-FIXAMT*32,me->y-4*FIXAMT+sway[me->frm]*FIXAMT,me->facing,BLT_DEATHBEAM,me->friendly);
			FireBullet(me->x-FIXAMT*32,me->y+4*FIXAMT+sway[me->frm]*FIXAMT,me->facing,BLT_DEATHBEAM,me->friendly);
			if(me->reload==0)
			{
				MakeSound(SND_INCABEAM,me->x,me->y,SND_CUTOFF,100);
				me->reload=10;
			}
		}
		if(me->facing==2 && goodguy->y>me->y && abs(goodguy->x-me->x)<48*FIXAMT)
		{
			FireBullet(me->x-8*FIXAMT+sway[me->frm]*FIXAMT,me->y+FIXAMT*16,me->facing,BLT_DEATHBEAM,me->friendly);
			FireBullet(me->x+8*FIXAMT+sway[me->frm]*FIXAMT,me->y+FIXAMT*16,me->facing,BLT_DEATHBEAM,me->friendly);
			if(me->reload==0)
			{
				MakeSound(SND_INCABEAM,me->x,me->y,SND_CUTOFF,100);
				me->reload=10;
			}
		}
		if(me->facing==6 && goodguy->y<me->y && abs(goodguy->x-me->x)<48*FIXAMT)
		{
			FireBullet(me->x-8*FIXAMT-sway[me->frm]*FIXAMT,me->y-FIXAMT*16,me->facing,BLT_DEATHBEAM,me->friendly);
			FireBullet(me->x+8*FIXAMT-sway[me->frm]*FIXAMT,me->y-FIXAMT*16,me->facing,BLT_DEATHBEAM,me->friendly);
			if(me->reload==0)
			{
				MakeSound(SND_INCABEAM,me->x,me->y,SND_CUTOFF,100);
				me->reload=10;
			}
		}
	}
}



void AI_Slug(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->mind2==0){
	me->facing=Random(7);
	me->mind2=1;
	}
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_SNAILDIE,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SNAILHIT,me->x,me->y,SND_CUTOFF,1200);
		me->facing=(me->facing+1-MGL_random(3))&7;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && MGL_random(7)==0)
		{
			FireBullet(me->x-FIXAMT*40+MGL_randoml(FIXAMT*81),me->y-FIXAMT*30+MGL_randoml(FIXAMT*61),0,BLT_SLIME,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind1)	// bumped a wall
	{
		me->facing=(byte)MGL_random(8);
		me->mind1=0;
	}

	if(me->reload==0 && goodguy && RangeToTarget(me,goodguy)<800*FIXAMT)
	{
		me->reload=10;
		FireBullet(me->x,me->y,0,BLT_SLIME,me->friendly);
	}

	me->dx=Cosine(me->facing*32)*2;
	me->dy=Sine(me->facing*32)*2;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}



void AI_Snail(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i,x,y;
	byte b;

	if(me->type==MONS_SNAIL2)
	{
		if(goodguy && RangeToTarget(me,goodguy)>(260*FIXAMT))
		{
			MakeSound(SND_SNAILOUTSHELL,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A2;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->type=MONS_SNAIL;
		}

		return;
	}

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_SNAILDIE,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SNAILHIT,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_A1 && me->frm==10)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_IDLE;
			me->type=MONS_SNAIL2;
		}

		if(me->seq==ANIM_ATTACK && me->frm>=3 && me->frm<=5)
		{
			// fire lots of acid
			for(i=0;i<2;i++)
			{
				x=me->x+Cosine(me->facing*32)*20-FIXAMT*4+MGL_randoml(FIXAMT*4+1);
				y=me->y+Sine(me->facing*32)*20-FIXAMT*4+MGL_randoml(FIXAMT*4+1);
				b=me->facing*32-4+MGL_random(9);
				FireExactBullet(x,y,FIXAMT*15,Cosine(b)*12,Sine(b)*12,FIXAMT*6,0,60,((b+16)&255)/32,BLT_ACID,me->friendly);
			}
		}
		if(me->seq==ANIM_DIE)
			me->frmAdvance=256;
		return;
	}

	if(goodguy && RangeToTarget(me,goodguy)<(240*FIXAMT))
	{
		// hide in your shell!
		MakeSound(SND_SNAILINSHELL,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=256;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(MGL_random(64)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && MGL_random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)MGL_random(8);
			me->mind1=MGL_random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}

	// spit if close enough and you want to
	if(goodguy && RangeToTarget(me,goodguy)<500*FIXAMT && me->reload==0)
	{
		FaceGoodguy(me,goodguy);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=60;
	}
}



void AI_Peeper(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_PEEPOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_PEEPDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			// fire a shot
			FireBullet(me->x,me->y,me->facing,BLT_BIGYELLOW,me->friendly);
			MakeSound(SND_PEEPSHOOT,me->x,me->y,SND_CUTOFF,1200);
			me->reload=30*4;
		}
		if(me->seq==ANIM_DIE)
		{
			BlowSmoke(me->x-FIXAMT*16+MGL_randoml(FIXAMT*32),me->y-FIXAMT*16+MGL_randoml(FIXAMT*32),
				me->z,MGL_randoml(FIXAMT*2));
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// not awake yet
	{
		me->seq=ANIM_A2;	// sleep
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=64;

		if(goodguy && (RangeToTarget(me,goodguy)<256*FIXAMT || me->ouch>0))
		{
			me->mind=1;	// wake up
			me->action=ACTION_BUSY;	// but don't do anything while waking up
			me->mind1=4;
		}
	}
	else	// awake and pissed
	{
		if(me->mind1)
			me->mind1--;
		else if(goodguy)
		{
			FaceGoodguy2(me,goodguy);
			me->mind1=4;
		}

		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
		if(MGL_random(64)==0 && me->reload==0)
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
		}
		if(MGL_random(256)==0)
		{
			me->seq=ANIM_A1;	// blink
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
		}
	}
}



void AI_Lookeyloo(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_LOOKEYOUCH,me->x,me->y,SND_CUTOFF,1800);
		else
		{
			MakeSound(SND_LOOKEYDIE,me->x,me->y,SND_CUTOFF,2000);
		}
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			FireBullet(me->x-128*FIXAMT+MGL_randoml(256*FIXAMT),me->y-64*FIXAMT+MGL_randoml(128*FIXAMT),
				(byte)MGL_random(256),BLT_BOOM,me->friendly);
			ShakeScreen(20);
			//if(me->frm==4)
			//KillPinkeyes();
		}
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frm==9 && me->frmTimer<32)
			{
				LaunchMegabeam(me->x,me->y+FIXAMT*40,me->ID);
			}
			if(me->frm==18)
				me->reload=150;
		}
		return;	// can't do nothin' right now
	}

	if(me->reload==0)
	{
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=32;
		me->action=ACTION_BUSY;
	}
}



void AI_Pinkeye(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_PEEPOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
		{
			MakeSound(SND_PEEPDIE,me->x,me->y,SND_CUTOFF,1200);
			me->reload=255;	// set the resurrection timer
		}
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			if(me->frm==4 && me->reload>0)
			{
				me->frm=3;	// stay still until you're done reloading!
			}
			if(me->frm==8)
			{
				// resurrect!
				me->hp=20;
				me->frm=0;
				me->seq=ANIM_IDLE;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_IDLE;
			}
		}
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frm==5 && me->frmTimer<32)
			{
				FireBullet(me->x,me->y,(byte)((me->facing*32)-16+MGL_random(32)),BLT_ENERGY,me->friendly);
				//MakeSound(SND_PEEPSHOOT,me->x,me->y,SND_CUTOFF,1500);
			}
		}
		return;	// can't do nothin' right now
	}

	if(goodguy)
		FaceGoodguy(me,goodguy);
	if(me->reload==0)
	{
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->reload=30+MGL_random(90);
	}
}

void AI_CrazyBush(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_BUSHOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_BUSHDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		if(RangeToTarget(me,goodguy)<(50*FIXAMT) && MGL_random(8)==0 && map->FindGuy(me->mapx,me->mapy,5,goodguy))
		{
			// get him!
			goodguy->GetShot(me->dx/2,me->dy/2,1,map,world);
		}
		FaceGoodguy(me,goodguy);
		//FaceGoodguy2(me,goodguy);

		me->dx=Cosine(me->facing*32)*5;
		me->dy=Sine(me->facing*32)*5;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_PeepBomb(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(!goodguy)
		return;

	if(me->reload)
		me->reload--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			me->frmAdvance=256;
			me->facing=0;
		}
		return;
	}

	if(me->mind==0)	// spinning, looking for goodguy
	{
		if(me->type==MONS_PEEPBOMB)
		{
			me->mind1++;
		}
		else
		{
			me->mind1--;

		}
		me->facing=me->mind1/8;	// pick the frame

		if(PeepAtGuy(me->x,me->y,map,me->mind1))
		{
			MakeSound(SND_PEEPALARM,me->x,me->y,SND_CUTOFF|SND_ONE,3000);
			me->mind=1;
			me->dx=Cosine(me->mind1)*7;
			me->dy=Sine(me->mind1)*7;
			me->mind1=30;
		}
	}
	if(me->mind==1)	// chasing!
	{
		me->mind1--;
		if(!me->mind1)
		{
			me->mind=0;
			me->dx=0;
			me->dy=0;
			me->mind1=me->facing*8;
		}
		else
		{
			FaceGoodguy(me,goodguy);
			me->facing*=4;
			me->dx=Cosine(me->facing*8 )*7;
			me->dy=Sine(me->facing*8)*7;
			if(RangeToTarget(me,goodguy)<FIXAMT*64)
			{
				me->hp=0;
				me->facing=0;
				me->seq=ANIM_DIE;
				me->frmAdvance=256;
				me->frm=0;
				me->frmTimer=0;
				me->dx=0;
				me->dy=0;
				MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,1200);
				goodguy->GetShot(-Cosine(me->facing*8)*20,-Sine(me->facing*8)*20,30,map,world);
				me->action=ACTION_BUSY;
			}
		}
	}
}

void AI_Rammy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_GOATHIT,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_GOATDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(!goodguy)
		return;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_MOVE)
		{
			if(me->mind<20)
			{
				me->mind++;
				me->mind1=0;	// don't let him bump stuff when slow
			}
			else
				if(FindVictim((me->x+Cosine(me->facing*32)*40)/FIXAMT,(me->y+Sine(me->facing*32)*30)/FIXAMT,40,
						Cosine(me->facing*32)*10,Sine(me->facing*32)*10,12,map,world, me->friendly) || me->mind1)
				{
					// make THUD noise
					MakeSound(SND_GOATDIZZY,me->x,me->y,SND_CUTOFF,1200);
					me->type=MONS_GOAT1B;	// become vulnerable
					me->seq=ANIM_A1;	// get stunned
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=128;
					me->action=ACTION_BUSY;
					me->dx=0;
					me->dy=0;
					ShakeScreen(10);
					me->mind1=0;
					return;
				}

			me->dx+=Cosine(me->facing*32)/2;
			me->dy+=Sine(me->facing*32)/2;
			Clamp(&me->dx,FIXAMT*8);
			Clamp(&me->dy,FIXAMT*8);
			if(me->reload==0)
			{
				me->reload=10;
				FaceGoodguy2(me,goodguy);
			}
			if(me->frm==12)
			{
				me->frm=0;
				MakeSound(SND_GOATRUN,me->x,me->y,SND_CUTOFF,1200);
			}
			if(MGL_random(3)==0)
				BlowSmoke(me->x-me->dx-FIXAMT*12+MGL_randoml(FIXAMT*24+1),
			me->y-me->dy-FIXAMT*12+MGL_randoml(FIXAMT*24+1),0,FIXAMT);
		}
		if(me->seq==ANIM_A1 && me->frm==22)
		{
			me->type=MONS_GOAT1;	// invulnerable again
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_IDLE;
		}
		return;	// can't do nothin' right now
	}

	FaceGoodguy(me,goodguy);

	if(MGL_random(48)==0)
	{
		MakeSound(SND_GOATRUN,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=10;
		me->mind=0;
		return;
	}
}

void AI_Hoppy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(!goodguy)
		return;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_GOATHIT,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_GOATDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->type==MONS_GOAT2B)
		me->action=ACTION_BUSY;

	if(me->action==ACTION_BUSY)
	{
		if(me->z>0 )
			me->dz+=FIXAMT/4;
		if(me->seq==ANIM_ATTACK && me->frm==3)
		{
			me->type=MONS_GOAT2B;
			me->dz=FIXAMT*20;
			me->dx=Cosine(me->facing*32)*5;
			me->dy=Sine(me->facing*32)*5;
			me->z=FIXAMT;
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}

		if(me->z==0 && me->type==MONS_GOAT2B && me->seq==ANIM_IDLE)
		{
			// thud sound again
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,1200);
			ShakeScreen(10);
			me->type=MONS_GOAT2;
			me->seq=ANIM_A1;	// squish onto the ground
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->dx=0;
			me->dy=0;
			me->mind1=0;
			FireBullet(me->x,me->y,0,BLT_SHOCKWAVE,me->friendly);
		}

		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<(300*FIXAMT) && MGL_random(16)==0)
	{
		// get him!
		FaceGoodguy(me,goodguy);
		MakeSound(SND_GOATSHOOT,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}

	if(!me->mind1)	// time to get a new direction
	{
		me->facing=(byte)MGL_random(8);
		me->mind1=MGL_random(40)+1;
		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
	}
	else
		me->mind1--;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_Gruffy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_GOATHIT,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_GOATDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==2 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*24;
			y=me->y+Sine(me->facing*32)*24;
			FireBullet(x,y,(byte)(me->facing*32-8+MGL_random(17)),BLT_BIGYELLOW,me->friendly);
			me->reload=5;
			me->mind1=1;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(512*FIXAMT) && MGL_random(64)==0)
			{
				// spit at him
				MakeSound(SND_GOATSHOOT,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				FaceGoodguy(me,goodguy);
				return;
			}

			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(MGL_random(64)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(512*FIXAMT) && MGL_random(64)==0)
			{
				// spit at him
				MakeSound(SND_GOATSHOOT,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				FaceGoodguy(me,goodguy);
				return;
			}
		}
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && MGL_random(10)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)MGL_random(8);
			me->mind1=MGL_random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_StickShroom(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_GINGEROUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_GINGERDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			i=(me->facing*32-8+MGL_random(17))&255;
			if(i<0)
				i+=256;
			x=me->x+Cosine(i)*4;
			y=me->y+Sine(i)*4;
			FireExactBullet(x,y,FIXAMT*20,Cosine(i)*8,Sine(i)*8,0,0,16,i,BLT_SPORE,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, charge towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(128*FIXAMT) && MGL_random(8)==0)
			{
				// get him!
				MakeSound(SND_GINGERCOMBO,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				return;
			}
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*3;
			me->dy=Sine(me->facing*32)*3;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(MGL_random(128)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=(byte)MGL_random(100);
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, sit around
			me->mind1=(byte)MGL_random(100);
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy && RangeToTarget(me,goodguy)<(128*FIXAMT) && MGL_random(8)==0)
		{
			// get him!
			MakeSound(SND_GINGERCOMBO,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}
		FaceGoodguy2(me,goodguy);

		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && MGL_random(10)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)MGL_random(8);
			me->mind1=MGL_random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*3;
		me->dy=Sine(me->facing*32)*3;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_StickSpider(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SPD2OUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SPD2DIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->reload==0)
		{
			i=(me->facing*32-8+MGL_random(17))&255;
			if(i<0)
				i+=256;
			x=me->x+Cosine(i)*4;
			y=me->y+Sine(i)*4;
			FireBullet(x,y,i,BLT_ACID,me->friendly);
			me->reload=2;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, charge towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(128*FIXAMT) && MGL_random(8)==0)
			{
				// get him!
				MakeSound(SND_GINGERCOMBO,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				return;
			}
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*3;
			me->dy=Sine(me->facing*32)*3;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(MGL_random(128)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=(byte)MGL_random(100);
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, sit around
			me->mind1=(byte)MGL_random(100);
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy && RangeToTarget(me,goodguy)<(128*FIXAMT) && MGL_random(8)==0)
		{
			// get him!
			MakeSound(SND_GINGERCOMBO,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}
		FaceGoodguy2(me,goodguy);

		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && MGL_random(10)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)MGL_random(8);
			me->mind1=MGL_random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*3;
		me->dy=Sine(me->facing*32)*3;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_StickCorpse(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SKELOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SKELDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			i=(me->facing*32-8+MGL_random(17))&255;
			if(i<0)
				i+=256;
			x=me->x+Cosine(i)*4;
			y=me->y+Sine(i)*4;
			FireExactBullet(x,y,FIXAMT*50,Cosine(i)*8,Sine(i)*8,-FIXAMT,0,50,i,BLT_ENERGY,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, charge towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(256*FIXAMT) && MGL_random(16)==0)
			{
				// get him!
				MakeSound(SND_SKELSHOOT,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				return;
			}
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(MGL_random(10)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=(byte)MGL_random(100);
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, sit around
			me->mind1=(byte)MGL_random(100);
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy && RangeToTarget(me,goodguy)<(256*FIXAMT) && MGL_random(16)==0)
		{
			// get him!
			FaceGoodguy2(me,goodguy);
			MakeSound(SND_SKELSHOOT,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}

		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && MGL_random(40)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)MGL_random(8);
			me->mind1=MGL_random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
}

void AI_StickBat(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte f;
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_BATOUCH,me->x,me->y,SND_CUTOFF,1200);
	}
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			x=me->x+Cosine(me->facing*32)*4;
			y=me->y+Sine(me->facing*32)*4;
			f=me->facing*32-8+MGL_random(17);
			FireExactBullet(x,y,me->z,Cosine(f)*8,Sine(f)*8,0,0,24-MGL_random(4),f,BLT_FLAME2,me->friendly);
			MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,200);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly zip towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(128*FIXAMT) && MGL_random(4)==0)
			{
				// get him!
				MakeSound(SND_BATEYES,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				return;
			}

			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*6;
			me->dy=Sine(me->facing*32)*6;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(MGL_random(16)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=1;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=1;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && MGL_random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)MGL_random(8);
			me->mind1=MGL_random(40)+1;
		}

		me->dx=Cosine(me->facing*32)*6;
		me->dy=Sine(me->facing*32)*6;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=110;
		}
	}
}

void AI_Tower(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_TOWEROUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_TOWERDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)	// leaping attack
		{
			if(me->frm<4)
			{
				me->dx=0;
				me->dy=0;
				me->dz=0;
			}
			else if(me->frm==4)
			{
				me->dx=Cosine(me->facing*32)*12;
				me->dy=Sine(me->facing*32)*12;
				me->dz=10*FIXAMT;
			}
			else if(me->frm<8)
			{
				// stay the course
				Dampen(&me->dx,FIXAMT/8);
				Dampen(&me->dy,FIXAMT/8);
			}
			else if(me->frm==12 && me->frmTimer<32)
			{
				FireBullet(me->x,me->y-FIXAMT,0,BLT_SHOCKWAVE,me->friendly);
				MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,1500);
				ShakeScreen(10);
			}
			else
			{
				Dampen(&me->dx,FIXAMT/2);
				Dampen(&me->dy,FIXAMT/2);
			}
		}
		if(me->seq==ANIM_DIE)
		{
			if(me->frm>=4)
			{
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				BlowSmoke((x+me->rectx+MGL_random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+MGL_random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z,FIXAMT);
			}
		}
		if(me->seq==ANIM_A1)
		{
			if(me->frmTimer<32)
			{
				MakeSound(SND_MUSHMISSILE,me->x,me->y,SND_CUTOFF,1500);
				me->facing=(me->facing+1)&7;
				FireBullet(me->x,me->y,me->facing*32,BLT_GRENADE,me->friendly);
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<300*FIXAMT)
				me->mind=1;
		}
		if(me->ouch)
			me->mind=1;
	}
	else	// chase down Kid Mystic
	{
		if(goodguy)
		{
			if(me->mind2)
				me->mind2--;
			else
			{
				if(me->mind1)
				{
					me->mind2=8;
					me->facing=(me->facing+3+MGL_random(3))&7;
					me->mind1=0;
				}
				else
				{
					FaceGoodguy2(me,goodguy);
					me->mind2=4;
				}
			}

			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(RangeToTarget(me,goodguy)>200*FIXAMT && me->reload==0)
			{
				me->seq=ANIM_A1;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=120;
			}
			if(MGL_random(32)==0 || RangeToTarget(me,goodguy)<100*FIXAMT)
			{
				//leap!
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
			}
		}
		else
			me->mind=0;
	}
}

void AI_Golem(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	Guy *g;
	int x,y;

	g=goodguy;

	if(g==NULL)
	{
		// vanish if there is no goodguy to follow
		me->type=MONS_NONE;
		return;
	}

	if(me->reload)
		me->reload--;

	if(me->mind1)
		me->mind1--;

	if(me->mind2)
		me->mind2--;
	else if(me->hp>0)
	{
		me->mind2=15;
		//me->hp--;
		if(me->hp==0)
		{
			me->seq=ANIM_DIE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=64;
			me->action=ACTION_BUSY;
			me->ouch=4;
			me->dx=0;
			me->dy=0;
		}
	}

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_GOLEMOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_GOLEMDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5 && me->reload==0)
		{
			// smash
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,600);
			ShakeScreen(5);
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			FireBullet(x,y,me->facing,BLT_SHOCKWAVE,me->friendly);
			/*
			if(goodguy)
			{
				if(me->AttackCheck(48,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
					goodguy->GetShot(Cosine(me->facing*32)*6,Sine(me->facing*32)*6,15,map,world);
			}
			*/
			me->reload=60;
		}
		if(me->seq==ANIM_A1)	// forming
		{
			if(!me->CanWalk(me->x,me->y,map,world))
				me->type=MONS_NONE;
		}
		if(me->seq==ANIM_DIE)
		{
			if(me->frm>3)
			{
				x=me->x-32*FIXAMT+MGL_randoml(64*FIXAMT);
				y=me->y-32*FIXAMT+MGL_randoml(64*FIXAMT);
				BlowSmoke(x,y,me->z,0);
				if(me->frmTimer<32 && MGL_random(2)==0)
					FireExactBullet(x,y,0,0,0,0,0,3,0,BLT_BOMB,me->friendly);
			}
		}
		return;
	}
	if(me->mind==0)	// standard combat mode
	{
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frmAdvance=128;
		}
		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
		if(goodguy==NULL || goodguy->type==MONS_FRIENDLY || RangeToTarget(me,g)>512*FIXAMT)
		{
			// too far from goodguy, run home
			me->mind=1;
		}
		else
		{
			if(RangeToTarget(me,goodguy)>300*FIXAMT)	// too far to fight, go home
			{
				me->mind=1;
			}
			else
			{
				if(me->mind1==0)
				{
					FaceGoodguy2(me,goodguy);
					me->mind1=4;
				}
				if(me->reload==0 && RangeToTarget(me,goodguy)<96*FIXAMT)
				{
					me->action=ACTION_BUSY;
					me->seq=ANIM_ATTACK;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=128;
					me->dx=0;
					me->dy=0;
					MakeSound(SND_GOLEMPOUND,me->x,me->y,SND_CUTOFF,1200);
				}
			}
		}
	}
	else if(me->mind==1)	// running home mode
	{
		if(RangeToTarget(me,g)<128*FIXAMT)
		{
			if(goodguy && RangeToTarget(me,goodguy)<300*FIXAMT)
			{
				// got close enough, get violent again
				me->mind=0;
			}
			else
			{
				me->mind=2;	// close enough to the Kid, sit still
			}
		}
		if(me->mind1==0)
		{
			FaceGoodguy2(me,g);
			me->mind1=4;
		}
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frmAdvance=128;
		}
		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
	}
	else	// waiting patiently mode
	{
		if(me->mind1==0)
			me->mind1=4;
		if(goodguy && RangeToTarget(me,goodguy)<300*FIXAMT)
		{
			// enemy is near, get violent again
			me->mind=0;
		}
		else if(RangeToTarget(me,g)>256*FIXAMT)
		{
			me->mind=1;	// got too far from home!
		}
		if(me->seq!=ANIM_IDLE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
		me->dx=0;
		me->dy=0;
	}
}

void AI_BobbyKhan(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_BOBBYOUCH,me->x,me->y,SND_CUTOFF,1800);
		else
			MakeSound(SND_BOBBYDIE,me->x,me->y,SND_CUTOFF,2000);
	}

	if(!goodguy)
		return;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_A2 && me->frm>=3)
		{
			// spin attack, begin moving
			if(me->frm<10)
			{
				me->dx+=Cosine(me->facing*32);
				me->dy+=Sine(me->facing*32);
				Clamp(&me->dx,FIXAMT*12);
				Clamp(&me->dy,FIXAMT*12);
			}
			else
			{
				Dampen(&me->dx,FIXAMT);
				Dampen(&me->dy,FIXAMT);
			}
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;

			if(!me->mind1)
				switch(me->frm)
				{
					case 4:
						// if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
						if(AttackCheck2(x-180,y-100,x,y+80,goodguy))
						{
							goodguy->GetShot(-FIXAMT*8,FIXAMT*8,32,map,world);
							goodguy->dx=-FIXAMT*8;
							goodguy->dy=FIXAMT*8;
							me->mind1=1;
						}
						break;
					case 5:
						if(AttackCheck2(x-200,y-180,x,y,goodguy))
						{
							goodguy->GetShot(-FIXAMT*12,0,32,map,world);
							goodguy->dx=-FIXAMT*12;
							goodguy->dy=0;
							me->mind1=1;
						}
						break;
					case 6:
						if(AttackCheck2(x-150,y-120,x+40,y,goodguy))
						{
							goodguy->GetShot(-FIXAMT*8,-FIXAMT*8,32,map,world);
							goodguy->dx=-FIXAMT*8;
							goodguy->dy=-FIXAMT*8;
							me->mind1=1;
						}
						break;
					case 7:
						if(AttackCheck2(x,y-150,x+150,y-20,goodguy))
						{
							goodguy->GetShot(0,-FIXAMT*12,32,map,world);
							goodguy->dx=0;
							goodguy->dy=-FIXAMT*12;
							me->mind1=1;
						}
						break;
					case 8:
						if(AttackCheck2(x+20,y-100,x+230,y,goodguy))
						{
							goodguy->GetShot(FIXAMT*8,-FIXAMT*8,32,map,world);
							goodguy->dx=FIXAMT*8;
							goodguy->dy=-FIXAMT*8;
							me->mind1=1;
						}
						break;
					case 9:
						if(AttackCheck2(x,y,x+230,y+70,goodguy))
						{
							goodguy->GetShot(FIXAMT*12,0,32,map,world);
							goodguy->dx=FIXAMT*12;
							goodguy->dy=0;
							me->mind1=1;
						}
						break;
					case 3:
					case 10:
						if(AttackCheck2(x-120,y,x+45,y+160,goodguy))
						{
							goodguy->GetShot(FIXAMT*8,FIXAMT*8,32,map,world);
							goodguy->dx=FIXAMT*8;
							goodguy->dy=FIXAMT*8;
							me->mind1=1;
						}
						break;
					case 11:
						if(AttackCheck2(x-220,y,x-10,y+120,goodguy))
						{
							goodguy->GetShot(FIXAMT*12,0,32,map,world);
							goodguy->dx=0;
							goodguy->dy=FIXAMT*12;
							me->mind1=1;
						}
						break;
				}
		}

		if(me->seq==ANIM_A3 && (me->frm==9 || me->frm==10))
		{
			ShakeScreen(30);
			if(me->frm==9 && me->frmTimer<32)
				MakeSound(SND_SDZLPOUND,me->x,me->y,SND_CUTOFF,1200);

			for(i=0;i<15;i++)
			{
				x=me->x+((400-MGL_random(801))<<FIXSHIFT);
				y=me->y+((400-MGL_random(801))<<FIXSHIFT);
				FireExactBullet(x,y,FIXAMT*160+(MGL_random(40)<<FIXSHIFT),
								0,0,0,0,30,2,BLT_ROCK,me->friendly);
			}
		}
		if(me->seq==ANIM_DIE)
		{
			if(me->frm>=8)
			{
				x=me->x>>FIXSHIFT;
				y=me->y>>FIXSHIFT;
				BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
				BlowSmoke((x+me->rectx+MGL_random(me->rectx2-me->rectx))<<FIXSHIFT,
						  (y+me->recty+MGL_random(me->recty2-me->recty))<<FIXSHIFT,
						  me->z,FIXAMT);
			}
		}
		if(me->seq==ANIM_A4 && me->frm==0 && me->frmTimer<32)
		{
				for(x=0;x<6;x++)
				{
					i=MGL_random(256);
					FireExactBullet(me->x,me->y+FIXAMT*30,FIXAMT*30,Cosine(i)*4,Sine(i)*4,0,0,255,4*32,BLT_BIGYELLOW,me->friendly);
				}
		}
		if(me->seq==ANIM_ATTACK && me->frm==5 && me->frmTimer<32)
		{
			// slash for pain
			x=(me->x>>FIXSHIFT)-64;
			y=(me->y>>FIXSHIFT);
			if(AttackCheck2(x,y,x+72,y+225,goodguy))
			{
				goodguy->GetShot(0,FIXAMT*12,64,map,world);
				goodguy->dx=0;
				goodguy->dy=FIXAMT*12;
			}
		}

		return;	// can't do nothin' right now
	}

	if(me->reload==0)
	{
		x=RangeToTarget(me,goodguy);
		y=0;	// flags to decide which attack to do

		// spin ANY time he gets close
		if(x<180*FIXAMT)
		{
			y|=8;
			if(goodguy->x>me->x-64*FIXAMT && goodguy->x<me->x+64*FIXAMT &&
				goodguy->y>me->y)
			{
				y|=1;	// 1=slash attack
			}
		}
		else if(x<300*FIXAMT)	// medium range
		{
			// do the slash if he is conveniently in front of you
			if(goodguy->x>me->x-64*FIXAMT && goodguy->x<me->x+64*FIXAMT &&
				goodguy->y>me->y)
			{
				y|=1;	// 1=slash attack
			}
			y|=2;	// 2=rock stomp

			// only do block if player is below me
			if(goodguy->y>me->y)
				y|=4;	// 4=shield block
		}
		else	// long range
		{
			y|=2;	// rock stomp
			y|=8;	// spin attack from afar sometimes to close the gap
		}

		if(y!=0)
		{
			if((y&1) && (MGL_random(10)==0))	// slash attack is highly likely
			{
				MakeSound(SND_BOBBYSLASH,me->x,me->y,SND_CUTOFF,1200);
				me->reload=30;
				me->action=ACTION_BUSY;
				me->seq=ANIM_ATTACK;	// slash
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=64;
				me->dx=0;
				me->dy=0;
				me->mind=0;
				return;
			}
			if((y&2) && (MGL_random(40)==0))	// rock stomp is rare
			{
				MakeSound(SND_BOBBYSTOMP,me->x,me->y,SND_CUTOFF,1200);
				me->reload=60;
				me->action=ACTION_BUSY;
				me->seq=ANIM_A3;	// stomp
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->dx=0;
				me->dy=0;
				me->mind=0;
				return;
			}
			if((y&4) && (MGL_random(20)==0))	// shield block is less rare, but is superceded by rock stomp
			{
				me->reload=30;
				me->action=ACTION_BUSY;
				me->seq=ANIM_A1;	// block
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->dx=0;
				me->dy=0;
				return;
			}
			if((y&8) && (MGL_random(20)==0))	// spin attack is relatively likely
			{
				MakeSound(SND_BOBBYSPIN,me->x,me->y,SND_CUTOFF,1200);
				me->reload=60;
				me->action=ACTION_BUSY;
				me->seq=ANIM_A2;	// spin attack
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				FaceGoodguy(me,goodguy);
				me->dx=0;
				me->dy=0;
				me->mind=0;
				return;
			}
		}
	}
	if(me->ouch>2 && me->action==ACTION_IDLE && me->reload==0 && MGL_random(4)==0 && me->mind==0)
	{
		me->reload=60;
		me->action=ACTION_BUSY;
		me->seq=ANIM_A1;	// block
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->dx=0;
		me->dy=0;
		me->mind=1;
		return;
	}
	if(me->action==ACTION_IDLE)
	{
		FaceGoodguy2(me,goodguy);
		me->dx=Cosine(me->facing*32)*4;
		me->dy=Sine(me->facing*32)*4;
		me->mind1=0;
	}
	if(me->seq!=ANIM_MOVE && me->action==ACTION_IDLE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_Incaboss(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i;

	if(!goodguy)
		return;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_INCAHIT,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_INCADIE2,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->frm==7)
			me->frm=6;	// stay dying forever

		return;
	}

	if(goodguy->y<me->y+FIXAMT*64 && abs(goodguy->x-me->x)<FIXAMT*90 && me->mind>0)
		goodguy->GetShot(0,0,10,map,world);

	if(me->mind==0)	// not set up yet
	{
		if(goodguy && map->FindGuy(me->mapx,me->mapy,12,goodguy) && (player.levelNum==18 || goodguy->y<=50*TILE_HEIGHT*FIXAMT+(FIXAMT*8)))
		{
			me->mind=1;	// mode 1 = doing nothing
			me->dx=FIXAMT*2;
			me->facing=60;	// used as a timer
		}
		else
			return;	// wait til he is close
	}

	if(me->mind1)	// bumped a wall
	{
		me->mind1=0;

		me->dx=-me->dx;
	}

	me->facing--;
	if(me->facing==0)
	{
		me->mind++;
		if(me->mind==6)
			me->mind=1;
		me->facing=60;
		switch(me->mind)	// setup each mode
		{
			case 1:
				me->frmAdvance=128;

				break;
			case 2:	// fire breath
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				break;
			case 3:
				break;
			case 4:	// laser eyes
				me->seq=ANIM_A2;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				break;
			case 5:	// tongue
				me->seq=ANIM_A1;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=80;
				break;
		}
	}
	switch(me->mind)
	{
		case 1:	// doing nothing
			me->type=MONS_INCABOSS;
			break;
		case 2:	// fire breath
			// shoot flames
			me->type=MONS_INCABOSS;
			if(me->frm>=4 && me->frm<=15)
			{
				for(i=0;i<4;i++)
					FireBullet(me->x-FIXAMT*48+MGL_randoml(FIXAMT*96+1),me->y+FIXAMT*32,(byte)(2*32-8+MGL_random(17)),BLT_FLAME2, me->friendly);
			}

			if(me->seq==ANIM_IDLE)
				me->facing=1;
			break;
		case 3:	// waiting after fire
			me->type=MONS_INCABOSS;
			break;
		case 4:	// laser eyes
			me->type=MONS_INCABOSS;
			// fire lasers
			if(me->frm>=6 && me->frm<=16)
			{
				if(me->reload==0)
				{
					MakeSound(SND_INCABEAM,me->x,me->y,SND_CUTOFF,100);
					me->reload=10;
				}
				if(me->dx>0)
				{
					FireBullet(me->x-FIXAMT*48,me->y-FIXAMT,2,BLT_DEATHBEAM, me->friendly);
					FireBullet(me->x+FIXAMT*42,me->y-FIXAMT,2,BLT_DEATHBEAM, me->friendly);
				}
				else
				{
					FireBullet(me->x-FIXAMT*48,me->y-FIXAMT,2,BLT_DEATHBEAM, me->friendly);
					FireBullet(me->x+FIXAMT*42,me->y-FIXAMT,2,BLT_DEATHBEAM, me->friendly);
				}
			}
			if(me->seq==ANIM_IDLE)
				me->facing=1;
			break;
		case 5:	// tongue
			me->type=MONS_INCATONGUE;
			if(me->frm==7 && me->frmTimer<128)
				MakeSound(SND_INCATONGUE,me->x,me->y,SND_CUTOFF,100);
			break;

	}
}

void AI_OctoBoss(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i,x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_INCAHIT,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_INCADIE,me->x,me->y,SND_CUTOFF,1200);
	}

	for(i=0;i<8;i++)
	{
		x=me->x/FIXAMT-60+MGL_random(121);
		y=me->y/FIXAMT-60+MGL_random(121);
		//WaterRipple(x,y,MGL_random(32*20));
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=4 && me->frm<=11)
		{
			x=me->x-FIXAMT*30+MGL_randoml(60*FIXAMT+1);
			y=me->y+FIXAMT*70;
			FireBullet(x,y,2*32-14+MGL_random(29),BLT_ENERGY, me->friendly);
			me->reload=60;
		}
		/*if(me->seq==ANIM_DIE && me->frm==5 && me->frmTimer<128)
			OctoBossDeath(map);*/

		return;	// can't do nothin' right now
	}

	if(goodguy)
	{
		x=goodguy->x;
		y=goodguy->y-100*FIXAMT;

		FaceGoodguy2(me,goodguy);
		if(me->x<x)
			me->dx+=FIXAMT/8;
		if(me->y<y)
			me->dy+=FIXAMT/8;
		if(me->x>x)
			me->dx-=FIXAMT/8;
		if(me->y>y)
			me->dy-=FIXAMT/8;

		if(me->mind1)
		{
			me->mind1=0;
			me->dy-=FIXAMT/4;	// move up if bumping an obstacle
		}
		Clamp(&me->dx,FIXAMT*3);
		Clamp(&me->dy,FIXAMT*3);

		if(me->reload==0 && (abs(me->x-x)<150*FIXAMT))	// within a reasonable horiz range
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
		}
	}
}

void AI_OctoTentacle(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(!me->parent || me->parent->hp==0)
	{
		me->GetShot(0,0,255,map,world);
	}

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_INCAHIT,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_INCADIE,me->x,me->y,SND_CUTOFF,1200);
	}

	//WaterRipple(me->x/FIXAMT,me->y/FIXAMT,MGL_random(32*20));

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==9 && me->reload==0)
		{
			if(!FindVictim((me->x+Cosine(me->facing*32)*160)/FIXAMT,(me->y+Sine(me->facing*32)*100)/FIXAMT,60,
				Cosine(me->facing*32)*10,Sine(me->facing*32)*10,8,map,world, me->friendly))
			{
				FindVictim((me->x+Cosine(me->facing*32)*40)/FIXAMT,(me->y+Sine(me->facing*32)*30)/FIXAMT,80,
					Cosine(me->facing*32)*10,Sine(me->facing*32)*10,8,map,world, me->friendly);
			}
			me->reload=4;
		}

		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly lumber towards Bouapha
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(200*FIXAMT) && MGL_random(8)==0)
			{
				// get him!
				FaceGoodguy(me,goodguy);
				MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				return;
			}
			FaceGoodguy2(me,goodguy);

			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			if(MGL_random(20)==0)
			{
				me->mind=1;		// occasionally wander
				me->mind1=10;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->mind1=10;
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(goodguy)
		{
			if(RangeToTarget(me,goodguy)<(200*FIXAMT) && MGL_random(8)==0)
			{
				// get him!
				FaceGoodguy(me,goodguy);
				MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
				return;
			}
		}
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && MGL_random(7)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)MGL_random(8);
			me->mind1=MGL_random(40)+1;
			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
		}
	}

	if(me->y<me->parent->y-FIXAMT*140)
	{
		me->dy=FIXAMT*4;
		me->mind=1;
		me->mind1=10;
	}
	if(me->y>me->parent->y+FIXAMT*140)
	{
		me->dy=-FIXAMT*4;
		me->mind=1;
		me->mind1=10;
	}
	if(me->type==MONS_OCTOTENT)	// left-side tentacle
	{
		if(me->x<me->parent->x-FIXAMT*220)
		{
			me->dx=FIXAMT*4;
			me->mind=1;
			me->mind1=10;
		}
		if(me->x>me->parent->x-FIXAMT*30)
		{
			me->dx=-FIXAMT*4;
			me->mind=1;
			me->mind1=10;
		}
	}
	else
	{
		if(me->x>me->parent->x+FIXAMT*220)
		{
			me->dx=-FIXAMT*4;
			me->mind=1;
			me->mind1=10;
		}
		if(me->x<me->parent->x+FIXAMT*30)
		{
			me->dx=FIXAMT*4;
			me->mind=1;
			me->mind1=10;
		}
	}

	if(RangeToTarget(me,me->parent)<FIXAMT*100)
	{
		if(me->x<me->parent->x)
			me->dx-=FIXAMT*6;
		if(me->x>me->parent->x)
			me->dx+=FIXAMT*6;
		if(me->y<me->parent->y)
			me->dy-=FIXAMT*6;
		if(me->y>me->parent->y)
			me->dy+=FIXAMT*6;
		me->mind=1;
		me->mind1=10;
	}
}

void AI_Microfish(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			ExplodeParticles(PART_SHORTYELLOW,me->x,me->y,me->z,2);
		}
		return;	// can't do nothin' right now
	}

	me->dx=Cosine(me->facing*32)*2;
	me->dy=Sine(me->facing*32)*2;

	if(me->mind==0)
	{
		me->mind=MGL_random(10)+1;
		me->facing=(byte)MGL_random(8);
	}
	else
		me->mind--;

}

void AI_Frog(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_FROGOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_FROGDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	/*
	if(me->type==MONS_FROG4 && Random(3)==0)
	{
		x=me->x-10*FIXAMT+Random(21*FIXAMT);
		y=me->y-10*FIXAMT+Random(21*FIXAMT);
		FireBullet(x,y,0,BLT_FLAMEWALL);
	}
	*/

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0 && me->type==MONS_FROG)
		{
			x=me->x+Cosine(me->facing*32)*30;
			y=me->y+Sine(me->facing*32)*30;
			if(me->AttackCheck(10,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,1,map,world);
			}
			else
			{
				x=me->x+Cosine(me->facing*32)*60;
				y=me->y+Sine(me->facing*32)*60;
				if(me->AttackCheck(10,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				{
					goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,1,map,world);
				}
			}
			me->reload=5;
		}
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0 && me->type==MONS_FROG2)
		{
			x=me->x+Cosine(me->facing*32)*10;
			y=me->y+Sine(me->facing*32)*10;
			FireBullet(x,y,me->facing*32-10+Random(21),BLT_ENERGY,me->friendly);
			me->reload=40;
		}
		/*
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0 && me->type==MONS_FROG2)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			FireBullet(x,y,me->facing*32,BLT_GASBLAST);
			me->reload=5;
		}
		if(me->seq==ANIM_ATTACK && me->frm==2 && me->reload==0 && me->type==MONS_FROG3)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			FireBullet(x,y,me->facing*32,BLT_ENERGY);
			me->reload=30;
		}
		if(me->seq==ANIM_DIE && me->type==MONS_FROG2 && me->frm==4)
		{
			for(y=0;y<4;y++)
			{
				x=Random(256);
				FireBullet(me->x,me->y,x,BLT_GASBLAST);
			}
		}
		if(me->seq==ANIM_DIE && me->type==MONS_FROG3 && me->frm==4 && me->reload==0)
		{
			FireBullet(me->x,me->y,0,BLT_FLAMEWALL);
			me->reload=5;
		}
		*/

		return;	// can't do nothin' right now
	}

	if(me->mind==0) // && me->type!=MONS_FROG4)	// heading for Bouapha
	{
		if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(8)==0 && me->type==MONS_FROG)
		{
			// get him!
			MakeSound(SND_FROGLICK,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
		if(RangeToTarget(me,goodguy)<(400*FIXAMT) && Random(8)==0 && me->type==MONS_FROG2 && me->reload==0)
		{
			// get him!
			MakeSound(SND_FROGLICK,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}
	}

	WanderAI(me,64,40,3,map,world,goodguy,FIXAMT*3);
}

void AI_MadCow(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_TRICEROUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_TRICERDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->frm>5)
		{
			me->facing=(me->facing+1)&7;
		}
		return;	// can't do nothin' right now
	}

	/*if(EatHay(me,1))
	{
		me->hp+=5;
		if(me->hp>me->maxHP)
			me->hp=me->maxHP;
		me->mind1=1;
	}*/

	x=me->maxHP-me->hp+5;	// more damaged=faster

	if(me->mind==0)
	{
		// walking
		if(goodguy && RangeToTarget(me,goodguy)<64*FIXAMT && map->FindGuy(me->mapx,me->mapy,5,goodguy))
		{
			goodguy->GetShot(-4*FIXAMT+Random(8*FIXAMT),-4*FIXAMT+Random(8*FIXAMT),10,map,world);
			me->dx=0;
			me->dy=0;
			me->mind=1;	// switch to turning mode
			me->reload=2;
			me->action=ACTION_BUSY;
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			MakeSound(SND_TRICERAROAR,me->x,me->y,SND_CUTOFF,1200);
			// pick a new direction
			if(Random(2)==0)
			{
				me->mind2=(me->facing+4+Random(3))&7;
				me->mind3=0;
			}
			else
			{
				me->mind2=(me->facing+4-Random(3))&7;
				me->mind3=2;
			}

			return;
		}
		if(me->mind1)
		{
			// bumped a wall
			MakeSound(SND_TRICERBUMP,me->x,me->y,SND_CUTOFF,1200);
			// pick a new direction
			if(Random(2)==0)
			{
				me->mind2=(me->facing+4+Random(3))&7;
				me->mind3=0;
			}
			else
			{
				me->mind2=(me->facing+4-Random(3))&7;
				me->mind3=2;
			}
			me->mind1=0;
			me->dx=0;
			me->dy=0;
			me->mind=1;	// switch to turning mode
			me->reload=2;

			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
		else
		{
			if((me->dx==0 && me->dy==0) || (me->ouch))
			{
				me->dx=Cosine(me->facing*32)*x/5;
				me->dy=Sine(me->facing*32)*x/5;
			}
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=(x/5)*30+40;
			}
		}
	}
	else
	{
		// turning
		if(!me->reload)
		{
			me->facing=(me->facing+me->mind3-1)&7;
			if(me->facing==me->mind2)	// done turning
			{
				me->mind=0;
			}
			me->reload=2;
		}
	}
}

void AI_MadCow2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_TRICEROUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_TRICERDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->hp<me->maxHP)
		me->hp=me->maxHP;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			x=me->x+Cosine(me->facing*32)*30;
			y=me->y+Sine(me->facing*32)*30;
			FireBullet(x,y,me->facing,BLT_FLAME2,0);
		}

		if(me->seq==ANIM_DIE && me->frm>5)
		{
			me->facing=(me->facing+1)&7;
		}
		return;	// can't do nothin' right now
	}

	/*if(EatHay(me,2))
	{
		me->type=MONS_MADCOW;
		me->hp=me->maxHP=MonsterHP(MONS_MADCOW);
		me->aiType=MONS_MADCOW;
		strcpy(me->name,"Annoyed Cow");
		MakeSound(SND_TRICERBUMP,me->x,me->y,SND_CUTOFF,1200);
		return;
	}*/

	if(Random(40)==0)
	{
		me->dx=0;
		me->dy=0;
		me->reload=2;
		me->action=ACTION_BUSY;
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		MakeSound(SND_TRICERAROAR,me->x,me->y,SND_CUTOFF,1200);
		if(Random(2))
			me->facing=(me->facing+1)&7;
		else
			me->facing=(me->facing-1)&7;
		return;
	}
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_PumpkinFly(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i;

	if(me->reload)
		me->reload--;

	if(me->type==MONS_PUMPKINFLY)
		map->DimTorch(me->mapx,me->mapy,2);
	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->reload==0)
		{
			for(i=0;i<10;i++)
				FireBullet(me->x,me->y,0,BLT_BADSITFLAME,me->friendly);
			me->reload=100;
			if(me->type==MONS_PUMPKINFLY2)
				FireBullet(me->x,me->y,0,BLT_BOOM,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(me->type==MONS_PUMPKINFLY2 && me->mind2<255)
	{
		me->mind2++;
	}

	if(me->type==MONS_PUMPKINFLY2 && me->mind1 && me->mind2>10)
	{
		// explode on impact
		//SmashTrees(me,2);
		me->hp=1;
		me->GetShot(0,0,1,map,world);
		return;
	}


	if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,32,0,0,0,map,world,me->friendly))
	{
		//if(me->type==MONS_PUMPKINFLY2)
			//SmashTrees(me,2);
		me->hp=1;
		me->GetShot(0,0,1,map,world);
		return;
	}

	if(Random(4)==0)
		FireBullet(me->x,me->y,Random(256),BLT_FLAME3,me->friendly);

	FaceGoodguy(me,goodguy);
	if(!goodguy)
	{
		return;
	}

	me->dx=Cosine(me->facing*32)*4;
	me->dy=Sine(me->facing*32)*4;

	if(me->type==MONS_PUMPKINFLY2)
	{
		me->dx*=2;
		me->dy*=2;
	}

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_Tree(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_PYGMYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_PYGMYDIE,me->x,me->y,SND_CUTOFF,1200);
		if(me->seq==ANIM_IDLE)
		{
			me->seq=ANIM_A1;
			me->frmTimer=0;
			me->frm=0;
			me->frmAdvance=32;
		}
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->frm==7)
		{
			for(x=0;x<16;x++)
				FireBullet(me->x,me->y,x*16,BLT_ENERGY,me->friendly);
			BlowUpGuy(me->x,me->y,me->x+1,me->y+1,0,1);
		}
		return;	// can't do nothin' right now
	}

	me->mind++;
	if(me->mind>=4)
	{
		me->mind=0;
		MakeSound(SND_BULLETFIRE,me->x,me->y,SND_CUTOFF,100);
		x=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
		FireExactBullet(me->x,me->y,FIXAMT*90,Cosine(x)*6,Sine(x)*6,-FIXAMT*2,0,90,(byte)x,BLT_ENERGY,me->friendly);
	}
}

void AI_Darkness(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->hp>0)
		map->BrightTorch(me->mapx,me->mapy,-16,2);

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_PUMPKINDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->reload==0)
		{
			map->BrightTorch(me->mapx,me->mapy,10,6);
			me->reload=100;
		}
		return;	// can't do nothin' right now
	}

	if(me->reload==0 && FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,32,Random(FIXAMT*2),Random(FIXAMT*2),1,map,world,me->friendly))
	{
		MakeSound(SND_ALIENOUCH,me->x,me->y,SND_CUTOFF,200);
		me->reload=2;
	}

	if(Random(4)==0)
		me->facing=(me->facing-1+Random(3))&7;

	me->dx=Cosine(me->facing*32)*4;
	me->dy=Sine(me->facing*32)*4;
}

void AI_Spark(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte wall[8];
	int i,c;
	if(!me->mind){
	me->mind=1;
	}
	if(me->reload)
		me->reload--;

	if(Random(2))
		ExplodeParticlesColor(7,me->x,me->y,me->z,1,1);

	// find out which sides have walls
	if(me->mapx<map->width-1)
	{
		wall[0]=(map->map[me->mapx+1+me->mapy*map->width].wall>0);
		if(me->mapy<map->height-1)
			wall[1]=(map->map[me->mapx+1+(me->mapy+1)*map->width].wall>0);
		else
			wall[1]=1;
		if(me->mapy>0)
			wall[7]=(map->map[me->mapx+1+(me->mapy-1)*map->width].wall>0);
		else
			wall[7]=1;
	}
	else
		wall[0]=1;
	if(me->mapy<map->height-1)
		wall[2]=(map->map[me->mapx+(me->mapy+1)*map->width].wall>0);
	else
		wall[2]=1;
	if(me->mapx>0)
	{
		wall[4]=(map->map[me->mapx-1+me->mapy*map->width].wall>0);
		if(me->mapy<map->height-1)
			wall[3]=(map->map[me->mapx-1+(me->mapy+1)*map->width].wall>0);
		else
			wall[3]=1;
		if(me->mapy>0)
			wall[5]=(map->map[me->mapx-1+(me->mapy-1)*map->width].wall>0);
		else
			wall[5]=1;
	}
	else
		wall[4]=1;
	if(me->mapy>0)
		wall[6]=(map->map[me->mapx+(me->mapy-1)*map->width].wall>0);
	else
		wall[6]=1;

	// shock neighboring walls for effect
	if(Random(3)==0)
	{
		if(wall[0])
			LightningBolt(me->x,me->y-me->z,((me->mapx+1)*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT,(me->mapy*TILE_HEIGHT+Random(TILE_HEIGHT))*FIXAMT-FIXAMT*20);
		if(wall[2])
			LightningBolt(me->x,me->y-me->z,(me->mapx*TILE_WIDTH+Random(TILE_WIDTH))*FIXAMT,((me->mapy+1)*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT-FIXAMT*20);
		if(wall[4])
			LightningBolt(me->x,me->y-me->z,((me->mapx-1)*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT,(me->mapy*TILE_HEIGHT+Random(TILE_HEIGHT))*FIXAMT-FIXAMT*20);
		if(wall[6])
			LightningBolt(me->x,me->y-me->z,(me->mapx*TILE_WIDTH+Random(TILE_WIDTH))*FIXAMT,((me->mapy-1)*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT-FIXAMT*20);
	}

	if(goodguy && RangeToTarget(me,goodguy)<48*FIXAMT && me->reload==0)
	{
		me->reload=20;
		LightningBolt(me->x,me->y-me->z,goodguy->x,goodguy->y-goodguy->z-FIXAMT*25);
		goodguy->GetShot(0,0,1,map,world);
	}

	if(me->mind==1)	// just entered a new space
	{
		me->mind1++;
		if(me->mind1==50)
		{
			//me->type=MONS_NONE;
			//return;
		}
		c=0;
		for(i=0;i<8;i++)
			if(wall[i])
				c++;

		if(c==0)	// no walls neighboring at all!
		{
			me->type=MONS_NONE;
			return;
		}

		me->mind=0;

		if(wall[(me->facing+2)&7])	// huggable wall
		{
			if(wall[(me->facing)])	// but forward motion blocked
			{
				if(wall[(me->facing+6)&7])	// and wrong-way blocked
					me->facing=(me->facing+4)&7;	// so turn around
				else
					me->facing=(me->facing+6)&7;
			}
			else
				me->facing=me->facing;
		}
		else
		{
			// huggable wall is missing
			if(wall[(me->facing+3)&7])
				me->facing=(me->facing+2)&7;	// can cornerhug
			else if(wall[(me->facing+6)&7])	// can hold onto opposite wall
			{
				if(wall[(me->facing)])	// but forward motion blocked
				{
					if(wall[(me->facing+2)&7])	// and right-way blocked
						me->facing=(me->facing+4)&7;	// so turn around
					else
						me->facing=(me->facing+2)&7;
				}
				else
					me->facing=me->facing;
			}
			else if(wall[(me->facing+5)&7])	// can't do that either, but can cornerhug the wrong way
				me->facing=(me->facing+6)&7;
			else if(wall[(me->facing)&7])	// headed straight into a wall
				me->facing=(me->facing+2)&7;
			else
			{
				//me->type=MONS_NONE;
				//return;	// nothing to hug in any direction!
			}
		}
	}
	me->dx=Cosine(me->facing*32)*2;
	me->dy=Sine(me->facing*32)*2;
	if(me->facing==0 || me->facing==4)
	{
		if(me->y<(me->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT)
			me->dy+=FIXAMT;
		else
			me->dy-=FIXAMT;
	}
	if(me->facing==2 || me->facing==6)
	{
		if(me->x<(me->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT)
			me->dx+=FIXAMT;
		else
			me->dx-=FIXAMT;
	}
}

void AI_LightSwitch(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->seq==ANIM_ATTACK)
	{
		if(me->frm>7)
			me->frm=7;
		map->DimTorch(me->mapx,me->mapy,3);
		//if(Random(3)==0)
			map->BrightTorch(me->mapx,me->mapy,16,2);
	}
}

void AI_LightBlaster(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	byte a;
	if(!me->mind){
	me->mind=1;
	}
	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ROBOOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_ROBODIE,me->x,me->y,SND_CUTOFF,1200);
	}
	if(me->reload)
		me->reload--;

	if(me->seq==ANIM_DIE)
	{
		x=me->x>>FIXSHIFT;
		y=me->y>>FIXSHIFT;
		BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
	}
	if(me->seq==ANIM_ATTACK)
	{
		if(me->frm==7 && me->reload==0)
		{
			MakeSound(SND_ROBOSHOOT,me->x,me->y,SND_CUTOFF,1200);
			me->frmAdvance=128;
			a=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
			FireExactBullet(me->x,me->y,FIXAMT*30,Cosine(a)*10,Sine(a)*10,0,0,60,a,BLT_BIGSHELL,me->friendly);
			FireExactBullet(me->x,me->y,FIXAMT*30,Cosine(a-4)*10,Sine(a-4)*10,0,0,60,a,BLT_BIGSHELL,me->friendly);
			FireExactBullet(me->x,me->y,FIXAMT*30,Cosine(a+4)*10,Sine(a+4)*10,0,0,60,a,BLT_BIGSHELL,me->friendly);

			map->BrightTorch(me->mapx,me->mapy,16,2);
			me->reload=10;
		}
		return;
	}
	if(me->seq==ANIM_IDLE)
	{
		if(RangeToTarget(me,goodguy)<FIXAMT*500)
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=20;
		}
	}
}

void AI_LightSlide(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int cx,cy;
	Guy *g;

	if(me->mind1>2)
		me->mind1-=3;

	if(me->mind1>0)
	{
		map->BrightTorch(me->mapx,me->mapy,(me->mind1/20),2);
		me->seq=ANIM_ATTACK;
		me->frm=(me->mind1/10);
		if(me->mind1>=98)
		{
			me->frm=0;
			g=AddGuy(me->x,me->y,0,MONS_SPARK,me->friendly);
			if(g)
			{
				g->item=ITM_NONE;
				g->facing=6;
				g->dx=0;
				g->dy=-2*FIXAMT;
			}
			me->mind1=0;
		}
		me->frmTimer=0;
		me->frmAdvance=0;
	}
	else
	{
		me->seq=ANIM_IDLE;
		me->frm=0;
		me->frmTimer=0;
	}

	SpreadCharge(me);

	if(me->mind==1)	// sliding
	{
		cx=(me->mapx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
		cy=(me->mapy*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;

		if(me->dx>0 && me->x>=cx && (me->mapx>=map->width-1 || !Walkable(me,me->mapx+1,me->mapy,map,world)))
		{
			me->x=cx;
			me->y=cy;
			me->dx=me->dy=0;
			me->mind=0;	// rest
		}
		if(me->dx<0 && me->x<=cx && (me->mapx<=0 || !Walkable(me,me->mapx-1,me->mapy,map,world)))
		{
			me->x=cx;
			me->y=cy;
			me->dx=me->dy=0;
			me->mind=0;	// rest
		}
		if(me->dy>0 && me->y>=cy && (me->mapy>=map->height-1 || !Walkable(me,me->mapx,me->mapy+1,map,world)))
		{
			me->x=cx;
			me->y=cy;
			me->dx=me->dy=0;
			me->mind=0;	// rest
		}
		if(me->dy<0 && me->y<=cy && (me->mapy<=0 || !Walkable(me,me->mapx,me->mapy-1,map,world)))
		{
			me->x=cx;
			me->y=cy;
			me->dx=me->dy=0;
			me->mind=0;	// rest
		}
		if(me->dx==0 && me->dy==0)
			me->mind=0;
	}
}

void AI_Grue(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	byte f;

	if(!goodguy)
		return;

	// heal in darkness
	if(map->map[me->mapx+me->mapy*map->width].templight<-3)
	{
		if(me->hp<me->maxHP)
		{
			i=me->hp;
			me->hp+=5;
			if(me->hp>me->maxHP)
				me->hp=me->maxHP;
			//ShowEnemyLife(me->name,i*128/me->maxHP,me->hp*128/me->maxHP,1);
		}
	}

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SPD3OUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SPD3DIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			if(Random(2))
			{
				AddBaby(me->x+((-8+Random(17))<<FIXSHIFT),
						me->y+((-8+Random(17))<<FIXSHIFT),0,MONS_SPIDER,me);
			}
		}
		if(me->seq==ANIM_A1 && me->frm==2 && me->reload==0 && goodguy)
		{
			FaceGoodguy(me,goodguy);
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*12,Sine(me->facing*32)*12,10,map,world);
				me->reload=2;
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind1)
		me->mind1--;

	if(map->map[goodguy->mapx+goodguy->mapy*map->width].templight<-8)	// if you are in the dark, you are good eats
	{
		me->mind=2;	// dash at you
		me->mind1=3;
	}

	switch(me->mind)
	{
		case 0:	// normal heading towards you
			FaceGoodguy(me,goodguy);
			if(me->mind1==0)
			{
				me->mind=1;
				me->facing=Random(8);
				me->mind1=10+Random(40);
			}
			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			me->frmAdvance=128;
			break;
		case 1:	// normal wandering
			if(me->mind1==0)
			{
				me->mind=0;
				me->mind1=10+Random(90);
			}
			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
			me->frmAdvance=128;
			break;
		case 2:	// dashing at you
			if(me->mind1==0)
			{
				me->mind=1;
				me->mind1=10+Random(40);
			}
			f=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
			me->facing=f/32;
			me->dx=Cosine(f)*8;
			me->dy=Sine(f)*8;
			me->frmAdvance=200;
			break;
	}

	if(!me->ignited)
	{
		if(Random(100)==0)
			AddBaby(me->x+((-8+Random(17))<<FIXSHIFT),me->y+((-8+Random(17))<<FIXSHIFT),0,MONS_SPIDER,me);
		if(Random(300)==0)
			AddBaby(me->x+((-8+Random(17))<<FIXSHIFT),me->y+((-8+Random(17))<<FIXSHIFT),0,MONS_BIGSPDR2,me);
	}

	if(goodguy)
	{

		x=RangeToTarget(me,goodguy);
		if(x<(128*FIXAMT) && Random(16)==0 && me->reload==0)
		{
			MakeSound(SND_SPD3CHOMP,me->x,me->y,SND_CUTOFF,1200);
			FaceGoodguy(me,goodguy);
			// bite him
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=200;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=1;
			return;
		}
	}

	if(me->mind!=2)	// when dashing, he ignores light, but otherwise wants to avoid it
	{
		x=(me->x+me->dx)/(TILE_WIDTH*FIXAMT);
		y=(me->y+me->dy)/(TILE_HEIGHT*FIXAMT);
		if(map->map[me->mapx+me->mapy*map->width].templight<-5 &&
			map->map[x+y*map->width].templight>=-5)	// you are in darkness now, but wouldn't be on moving
		{
			me->dx=0;
			me->dy=0;
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			return;
		}
		if(map->map[me->mapx+me->mapy*map->width].templight>=-5)	// if you are in light at any time, run
		{
			me->dx=(me->dx*3)/2;
			me->dy=(me->dy*3)/2;
			me->frmAdvance=160;
		}
	}

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_Spook(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;

	if(me->reload)
		me->reload--;
	
	if(me->type==MONS_SPOOKLEY)
	{
		// heal in ghost form
		if(me->mind2==0)//me->hp<me->maxHP &&)
		{
			i=me->hp;
			me->hp+=5;//std::min(me->hp+10,me->maxHP);
			
			if((me->hp>=me->maxHP) || (i == me->hp)){
				me->type=MONS_HAUNTMORE;
				me->hp=me->maxHP;
			}
			//ShowEnemyLife(me->name,i*128/me->maxHP,me->hp*128/me->maxHP,1);
		}
		map->DimTorch(me->mapx,me->mapy,6);
	}
	else
	{
		if(me->hp<(int)me->mind3)	// been hurt too much, ghost out
		{
			me->type=MONS_SPOOKLEY;
			me->ignited=0;
			MakeSound(SND_SPOOKSHIFT,me->x,me->y,SND_CUTOFF,2000);
			map->BrightTorch(me->mapx,me->mapy,16,6);
			if(me->mind3>100)
				me->mind3-=100;
			else
				me->mind3=0;
		}
	}
	if(me->dx==0 && me->dy==0)
	{
		me->dx=FIXAMT*4;
		me->dy=FIXAMT*4;
	}

	if(me->mind1&1)
		me->dx=-me->dx;
	if(me->mind1&2)
		me->dy=-me->dy;
	me->mind1=0;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_SPOOKDIE,me->x,me->y,SND_CUTOFF,2000);
		else
			MakeSound(SND_SPOOKOUCH,me->x,me->y,SND_CUTOFF,2000);
	}

	if(me->mind2==0)
	{
		me->mind2=5;
		FaceGoodguy(me,goodguy);
		x=me->facing*32+32-Random(65);
		if(x<0)
			x+=256;
		if(x>255)
			x-=256;
		FireBullet(me->x,me->y,x,BLT_ACID,me->friendly);
	}
	else
		me->mind2--;

	if(Random(10)==0)
		StinkySteam(me->x-FIXAMT*20+Random(FIXAMT*40),me->y-FIXAMT*20+Random(FIXAMT*40),
					me->z+FIXAMT*40,FIXAMT*2);
	if(Random(100)==0)
		AddBaby(me->x,me->y,me->z,MONS_GHOST,me);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
		}
		return;
	}

	if(goodguy)
	{
		if(me->AttackCheck(70,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy) && goodguy->poison==0)
		{
			MakeSound(SND_THINGVOMIT,me->x,me->y,SND_CUTOFF,1200);
			goodguy->poison=30;
		}
	}
	if(me->seq!=ANIM_IDLE)
	{
		me->seq=ANIM_IDLE;
		me->frm=0;
		me->frmTimer=0;
	}
}

void AI_Speedy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte a;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_WOLFOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_WOLFDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
			me->frmAdvance=190;
		return;	// can't do nothin' right now
	}

	if(me->mind==0 && (me->ouch || RangeToTarget(me,goodguy)<400*FIXAMT))
	{
		me->mind=1;
		me->mind3=Random(256);
		me->facing=2;
		// howl
		MakeSound(SND_WOLFHOWL,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A3;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}

	if((me->mind1))	// shoot when you hit a wall
	{
		a=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
		FireExactBullet(me->x,me->y,me->z+FIXAMT*20,Cosine(a)*6,Sine(a)*6,0,0,60,a,BLT_BIGSHELL,me->friendly);
	}
	if(RangeToTarget(me,goodguy)<FIXAMT*30)
		goodguy->GetShot(Cosine(me->mind3+64)*10,Sine(me->mind3+64)*10,4,map,world);
	if(me->mind1&1)
	{
		if(me->dx>0)
			me->mind3=64+Random(128);
		else
			me->mind3=192+Random(128);
	}
	if(me->mind1&2)
	{
		if(me->dy>0)
			me->mind3=128+Random(128);
		else
			me->mind3=Random(128);
	}
	me->facing=me->mind3/32;
	me->dx=Cosine(me->mind3)*18;
	me->dy=Sine(me->mind3)*18;
	me->mind1=0;

	if(me->seq==ANIM_MOVE)
		me->frmAdvance=256;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=256;
	}
}

void AI_IronSkull(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte f;
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_CLANG,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_MUSHDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			// still chase, but slowly
			x=goodguy->x;
			y=goodguy->y-96*FIXAMT;

			FaceGoodguy3(me,goodguy);
			if(me->x<x)
				me->dx+=FIXAMT/8;
			if(me->y<y)
				me->dy+=FIXAMT/8;
			if(me->x>x)
				me->dx-=FIXAMT/8;
			if(me->y>y)
				me->dy-=FIXAMT/8;

			Clamp(&me->dx,FIXAMT*1);
			Clamp(&me->dy,FIXAMT*1);

			if(me->frm==1 && me->frmTimer<64)
			{
				// shoot laser
				x=me->x;
				y=me->y+FIXAMT*8;
				LaunchMegabeam(x,y,me->ID);
			}
		}

		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
		}
		return;
	}

	if(me->mind==0)
	{
		if(goodguy && (RangeToTarget(me,goodguy)<400*FIXAMT || me->ouch>0))
		{
			me->mind=1;
		}
	}
	if(me->mind==1)	// active
	{
		if(Random(100)==0)
			AddBaby(me->x,me->y,0,MONS_BONEHEAD,me);

		if(goodguy)
		{
			// calculate desired location (want to be above Bouapha)
			x=goodguy->x;
			y=goodguy->y-96*FIXAMT;

			FaceGoodguy2(me,goodguy);
			if(me->x<x)
				me->dx+=FIXAMT/8;
			if(me->y<y)
				me->dy+=FIXAMT/8;
			if(me->x>x)
				me->dx-=FIXAMT/8;
			if(me->y>y)
				me->dy-=FIXAMT/8;

			Clamp(&me->dx,FIXAMT*3);
			Clamp(&me->dy,FIXAMT*4);
		}

		if(me->reload==8)
		{
			// left eye fires
			f=(me->facing*32-32+me->mind1)&255;
			// can't fire backwards
			if(f>128)
			{
				if(f<128+64)
					f=128;
				else
					f=0;
			}
			FireExactBullet(me->x-32*FIXAMT,me->y+32*FIXAMT,me->z+80*FIXAMT,
						Cosine(f)*8,Sine(f)*8,-3*FIXAMT,0,30,me->facing,BLT_ENERGY,me->friendly);
		}
		if(!me->reload)
		{
			// right eye fires
			f=(me->facing*32+32-me->mind1)&255;
			// can't fire backwards
			if(f>128)
			{
				if(f<128+64)
					f=128;
				else
					f=0;
			}
			FireExactBullet(me->x+32*FIXAMT,me->y+32*FIXAMT,me->z+80*FIXAMT,
						Cosine(f)*8,Sine(f)*8,-3*FIXAMT,0,30,me->facing,BLT_ENERGY,me->friendly);
			me->reload=16;
			me->mind1+=8;
			if(me->mind1>=64)
				me->mind1=0;
		}

		if(Random(128)==0)
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=64;
			me->action=ACTION_BUSY;
			return;
		}
	}

	if(me->seq!=ANIM_IDLE)
	{
		me->seq=ANIM_IDLE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_BigHead(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_MUSHOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_MUSHDIE,me->x,me->y,SND_CUTOFF,1200);

	}

	me->z=FIXAMT*55;
	me->dz=0;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frm<4)
				me->z-=FIXAMT*5*me->frm;
			else
				me->z-=FIXAMT*5*(7-me->frm);

			if(me->frm==3 && me->reload==0 && me->type==MONS_BIGHEAD)
			{
				FireExactBullet(me->x,me->y,me->z,0,FIXAMT*4,FIXAMT*8,0,120,64,BLT_GRENADE,me->friendly);
				me->reload=45;
			}
			if(me->type==MONS_BIGHEAD2)
			{
				x=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
				FireExactBullet(me->x,me->y,me->z,Cosine(x)*8,Sine(x)*8,-FIXAMT/2,0,60,x,BLT_BIGSHELL,me->friendly);
				me->reload=60;
			}
		}

		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
		}
		return;
	}

	if(me->mind==0)
	{
		if(goodguy && (RangeToTarget(me,goodguy)<400*FIXAMT || me->ouch>0))
		{
			me->mind=1;
		}
	}
	if(me->mind==1)	// active
	{
		if(goodguy)
		{
			// calculate desired location (want to be above Bouapha)
			x=goodguy->x;
			y=goodguy->y-96*FIXAMT;

			FaceGoodguy2(me,goodguy);
			if(me->x<x)
				me->dx+=FIXAMT/4;
			if(me->y<y)
				me->dy+=FIXAMT/4;
			if(me->x>x)
				me->dx-=FIXAMT/4;
			if(me->y>y)
				me->dy-=FIXAMT/4;

			Clamp(&me->dx,FIXAMT*5);
			Clamp(&me->dy,FIXAMT*5);
		}

		if(RangeToTarget(me,goodguy)<300*FIXAMT && goodguy->y>me->y && me->reload==0)
		{
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			return;
		}
	}

	if(me->seq!=ANIM_IDLE)
	{
		me->seq=ANIM_IDLE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_BigHeadBod(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->parent)
	{
		if(me->parent->hp==0 || me->parent->type==MONS_NONE)
			me->type=MONS_NONE;	// gone!

		me->x=me->parent->x;
		me->y=me->parent->y-1;
		me->z=0;
		if(me->parent->dx<FIXAMT)
		{
			me->facing=3;
		}
		else if(me->parent->dx>FIXAMT)
		{
			me->facing=1;
		}
		else
		{
			me->facing=2;
		}
	}
	else
		me->type=MONS_NONE;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_BigHead2(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	me->facing=(me->mind2/32);

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_MUSHOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_MUSHDIE,me->x,me->y,SND_CUTOFF,1200);

	}

	me->z=FIXAMT*55;
	me->dz=0;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frm<4)
				me->z-=FIXAMT*5*me->frm;
			else
				me->z-=FIXAMT*5*(7-me->frm);

			if(me->frm==3 && me->reload==0)
			{
				FireExactBullet(me->x,me->y,me->z,0,FIXAMT*4,FIXAMT*8,0,120,64,BLT_GRENADE,me->friendly);
				me->reload=45;
			}
		}

		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z,FIXAMT);
		}
		return;
	}

	if(RangeToTarget(me,goodguy)<90*FIXAMT)
	{
		goodguy->GetShot(0,0,4,map,world);
		me->mind=1;
		me->mind2+=128;
	}

	if(me->mind==0)
	{
		if(goodguy && (RangeToTarget(me,goodguy)<400*FIXAMT || me->ouch>0))
		{
			me->mind=Random(3)+1;
			me->mind1=0;
			me->mind2=Random(256);
		}
	}
	switch(me->mind)
	{
		case 1:	// go straight
			if(me->mind1)
			{
				me->mind2+=128;	// go the opposite way
				me->mind=2+Random(3);	// and be in one of the turny modes
			}
			else
			{
				if(Random(30)==0)
					me->mind=Random(4)+1;	// change modes sometimes
			}
			break;
		case 2:	// turning left
			if(me->mind1)
			{
				me->mind2+=128;	// go the opposite way
				me->mind=2+Random(3);	// and be in one of the turny modes
			}
			me->mind2-=4;
			if(Random(30)==0)
				me->mind=Random(4)+1;	// change modes sometimes
			break;
		case 3:	// turning right
			if(me->mind1)
			{
				me->mind2+=128;	// go the opposite way
				me->mind=2+Random(3);	// and be in one of the turny modes
			}
			me->mind2+=4;
			if(Random(30)==0)
				me->mind=Random(4)+1;	// change modes sometimes
			break;
		case 4: // chasing
			if(me->mind1)
			{
				me->mind2+=128;	// go the opposite way
				me->mind=2+Random(3);	// and be in one of the turny modes
			}
			byte a=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
			me->mind2=TurnToward(me->mind2,a,4);
			if(Random(30)==0)
				me->mind=Random(4)+1;	// change modes sometimes
			break;
	}
	me->mind1=0;
	me->dx=Cosine(me->mind2)*7;
	me->dy=Sine(me->mind2)*7;
	if(Random(3)==0)
	{
		byte a;

		a=Random(256);
		FireExactBullet(me->x,me->y,FIXAMT*10,Cosine(a)*2,Sine(a)*2,FIXAMT,0,30*6,0,BLT_BADSITFLAME,me->friendly);
	}
}

void AI_Rafe(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->ignited)
		me->ignited=0;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
		{
			MakeSound(SND_GLASSHIT,me->x,me->y,SND_CUTOFF,1200);
		}
		else
			MakeSound(SND_GLASSDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5 && me->reload==0)
		{
			// hack at him with axe
			x=me->x-FIXAMT*20;
			y=me->y+FIXAMT*60;

			FindVictim(x>>FIXSHIFT,y>>FIXSHIFT,60,0,FIXAMT*8,8,map,world,me->friendly);

			me->reload=10;

		}
		if(me->seq==ANIM_A1 && me->frm==13 && me->reload==0)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			GlassShatter(x-40,y-100,x+40,y+20,me->z,50);
			MakeSound(SND_GLASSBREAK,me->x,me->y,SND_CUTOFF,1200);
			me->reload=3;
		}
		if(me->seq==ANIM_DIE && me->frm==12 && me->reload==0)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			GlassShatter(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,100);
			MakeSound(SND_GLASSBREAK,me->x,me->y,SND_CUTOFF,1200);
			me->reload=3;
		}
		return;	// can't do nothin' right now
	}
	if(Random(100)==0)
		AddBaby(me->x,me->y,0,MONS_GLASSJAW,me);

	if(me->mind==0)
	{
		if(goodguy && (RangeToTarget(me,goodguy)<600*FIXAMT || me->ouch>0))
		{
			me->mind=1;
		}
	}
	else
	{
		if(goodguy)
		{
			me->dx=0;
			me->dy=0;
			if(goodguy->y<me->y+FIXAMT*30)
				me->dy=-FIXAMT*6;
			else if(goodguy->y>me->y+FIXAMT*128)
				me->dy=FIXAMT*3;
			if(goodguy->x<me->x-FIXAMT*40)
				me->dx=-FIXAMT*3;
			if(goodguy->x>me->x+FIXAMT*40)
				me->dx=FIXAMT*3;

			if(me->dx==0 && me->dy==0)
			{
				if(RangeToTarget(me,goodguy)>72*FIXAMT)
				{
					if(goodguy->x>me->x+FIXAMT*2)
						me->dx=FIXAMT*2;
					else if(goodguy->x<me->x-FIXAMT*2)
						me->dx=-FIXAMT*2;
					if(goodguy->y>me->y+FIXAMT*2)
						me->dy=FIXAMT*2;
					else if(goodguy->y<me->y-FIXAMT*2)
						me->dy=-FIXAMT*2;
				}
			}
			if(RangeToTarget(me,goodguy)<128*FIXAMT && goodguy->y>me->y && Random(10)==0)
			{
				MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
				me->dx=0;
				me->dy=0;
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=256;
				me->action=ACTION_BUSY;
				return;
			}
		}
		if(me->dx!=0 || me->dy!=0)
		{
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
		}
		else
		{
			if(me->seq!=ANIM_IDLE)
			{
				me->seq=ANIM_IDLE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
		}
	}
}

void AI_Spatula(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	Guy *g;
	int x,y;
	byte minions;

	if(!goodguy)
		return;

	minions=ArrangeBats(AngleFrom(me->x,me->y,goodguy->x,goodguy->y),me);

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_COUNTOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_COUNTDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>5 && me->frm<9 && me->reload==0)
		{
			me->reload=30;
			if(minions<8)
			{
				g=AddBaby(me->x,me->y+FIXAMT*40,FIXAMT*20,MONS_BATGUARD,me);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					RemoveGuy(g);
				else if(g)
				{
					g->mind=64;	// current angle
					g->parent=me;
				}
			}
		}

		if(me->seq==ANIM_DIE && minions>0)
		{
			if(me->reload>4)
				me->reload=4;

			if(me->reload==0)
			{
				if(minions>0)
				{
					g=GetChildren(me);
					if(g!=NULL && g->parent==me)
					{
						g->hp=1;
						g->GetShot(0,0,100,map,world);
					}
				}
				me->reload=4;
			}
		}
		if(me->seq==ANIM_DIE && me->frm>3)
		{
			me->frmAdvance=128;
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowSmoke((x+me->rectx+Random(me->rectx2-me->rectx))<<FIXSHIFT,
					  (y+me->recty+Random(me->recty2-me->recty))<<FIXSHIFT,
					  me->z-Random(FIXAMT*60),FIXAMT);
		}

		return;	// can't do nothin' right now
	}

	if(!goodguy)
		return;

	if(minions<8 && me->reload==0)
	{
		MakeSound(SND_COUNTSHOOT,me->x,me->y,SND_CUTOFF,1200);
		me->action=ACTION_BUSY;
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		return;
	}
	if(me->mind==0)	// waiting for Bouapha
	{
		if(RangeToTarget(me,goodguy)<300*FIXAMT || me->ouch>0)
		{
			MakeSound(SND_COUNTHELLO,me->x,me->y,SND_CUTOFF,1200);
			me->mind=1;
		}
		me->mind1=60;
	}
	if(me->mind==1)	// wandering mode
	{
		// calculate desired location (want to be above Bouapha)
		x=goodguy->x;
		y=goodguy->y;

		FaceGoodguy3(me,goodguy);
		if(me->x<x)
			me->dx+=FIXAMT/8;
		if(me->y<y)
			me->dy+=FIXAMT/8;
		if(me->x>x)
			me->dx-=FIXAMT/8;
		if(me->y>y)
			me->dy-=FIXAMT/8;

		Clamp(&me->dx,FIXAMT*3);
		Clamp(&me->dy,FIXAMT*4);
	}
	if(me->mind1==0)
	{
		me->mind1=120;
		me->mind2=30;	// while non-zero, all bats constantly shoot
	}
	else
		me->mind1--;

	if(me->mind2)
		me->mind2--;
}

void AI_BatGuard(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_BATOUCH,me->x,me->y,SND_CUTOFF,1200);
	}
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_A1 && me->frm<6 && me->reload==0)
		{
			FireBullet(me->x,me->y,me->mind,BLT_ENERGY,me->friendly);
			MakeSound(SND_MATTIESHOOT,me->x,me->y,SND_CUTOFF,200);
			me->reload=2;
		}
		if(me->seq==ANIM_A1  &&me->frm==5 && me->parent && me->parent->mind2!=0)
			me->frm=4;	// stay there until done

		return;	// can't do nothin' right now
	}

	if(me->AttackCheck(16,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
	{
		goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,4,map,world);
	}
	if(me->parent && me->parent->mind2>0)
	{
		MakeSound(SND_BATDIVE,me->x,me->y,SND_CUTOFF,200);
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		return;
	}
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=110;
	}
}

void AI_ScaredyBat(Guy *me,Map *map,world_t *world,Guy *badguy)
{
	int x;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp==0)
			MakeSound(SND_BATOUCH,me->x,me->y,SND_CUTOFF,1200);
	}
	if(me->action==ACTION_BUSY)
	{
		return;	// can't do nothin' right now
	}

	if(!goodguy)
		return;

	if(map->FindGuy(me->mapx,me->mapy,10,goodguy) && RangeToTarget(me,goodguy)<128*FIXAMT)
	{
		x=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
		x=(x+128)&255;	// face away!
		me->dx=Cosine(x)*8;
		me->dy=Sine(x)*8;
	}
	else if(badguy && map->FindGuy(me->mapx,me->mapy,10,badguy) && RangeToTarget(me,badguy)<128*FIXAMT)
	{
		x=AngleFrom(me->x,me->y,badguy->x,badguy->y);
		x=(x+128)&255;	// face away!
		me->dx=Cosine(x)*8;
		me->dy=Sine(x)*8;
	}
	else	// nobody too near, settle
	{
		me->dx=(me->dx*3)/4;
		me->dy=(me->dy*3)/4;
		if(me->z>0)
			me->dz-=FIXAMT;
		else
			me->dz=-FIXAMT;
	}

	if(badguy && RangeToTarget(me,badguy)<RangeToTarget(me,goodguy) && map->FindGuy(me->mapx,me->mapy,10,badguy))
		FaceGoodguy(me,badguy);
	else if(map->FindGuy(me->mapx,me->mapy,10,goodguy))
		FaceGoodguy(me,goodguy);

	if(me->dx==0 && me->dy==0)
	{
		if(me->z>FIXAMT)
		{
			if(me->frmAdvance>64)
				me->frmAdvance-=4;
			else
				me->frmAdvance+=4;
		}
		else
			if(me->frmAdvance>32)
				me->frmAdvance--;
			else if(me->frm==2)
				me->frmAdvance=0;
	}
	else
		me->frmAdvance=128;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
	}
}

void AI_Horseradish(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_MUSHOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_MUSHDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->hp<100)
		me->hp=10000;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==2 && me->reload==0 && goodguy)
		{

			AddBaby(me->x,me->y,FIXAMT*80,MONS_PUMPKINFLY2,me);
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	if(goodguy && RangeToTarget(me,goodguy)<48*FIXAMT)
		goodguy->GetShot(0,0,2,map,world);

	if(me->mind==0)
	{
		if(goodguy && RangeToTarget(me,goodguy)<600*FIXAMT)
			me->mind=2;
	}
	else if(me->mind==2)		// when mind=0, singlemindedly drift towards Bouapha
	{
		if(goodguy)
		{
			if(me->reload==0 && Random(50)==0)
			{
				MakeSound(SND_VAMPATTK,me->x,me->y,SND_CUTOFF,800);
				me->seq=ANIM_ATTACK;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=160;
				me->action=ACTION_BUSY;
				me->dx=me->dy=0;
				return;
			}
			FaceGoodguy(me,goodguy);

			me->dx=Cosine(me->facing*32)*6;
			me->dy=Sine(me->facing*32)*6;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=180;
			}
			if(me->mind1>0 && RangeToTarget(me,goodguy)>(64*FIXAMT))
			{
				me->mind=1;	// bumped a wall, so randomly maneuver
				me->facing=(byte)Random(8);
				me->mind2=10;
			}
		}
		else
		{
			me->mind=1;	// if there's no goodguy, get random
			me->facing=(byte)Random(8);
		}
	}
	else if(me->mind==1)	// random wandering
	{
		if(me->reload==0 && Random(50)==0)
		{
			MakeSound(SND_VAMPATTK,me->x,me->y,SND_CUTOFF,800);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=160;
			me->action=ACTION_BUSY;
			FaceGoodguy(me,goodguy);
			me->mind=2;
			me->dx=me->dy=0;
			return;
		}
		if(!(me->mind2--))	// time to get a new direction
		{
			me->mind=2;	// get back on track
		}

		me->dx=Cosine(me->facing*32)*6;
		me->dy=Sine(me->facing*32)*6;
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
	else
	{
		me->mind=2;
		return;
	}

	me->mind1=0;
}

void AI_Sparky(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_WOLFOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_WOLFDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
			me->frmAdvance=190;
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<FIXAMT*100 && me->reload==0)
	{
		me->reload=8;
		LightningBolt(me->x,me->y-FIXAMT*30,goodguy->x,goodguy->y-FIXAMT*30);
		goodguy->GetShot(Cosine(me->mind3+64)*10,Sine(me->mind3+64)*10,4,map,world);
	}
	else if(me->reload==0)
	{
		me->reload=8;
		LightningBolt(me->x,me->y-FIXAMT*30,me->x-FIXAMT*15+Random(FIXAMT*30+1),me->y-FIXAMT*30-FIXAMT*10+Random(FIXAMT*20+1));
	}

	if(me->mind==0)
	{
		if(me->ouch || RangeToTarget(me,goodguy)<200*FIXAMT)
		{
			me->mind=1;
			me->mind3=Random(256);
			me->facing=2;
			// howl
			MakeSound(SND_WOLFHOWL,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A3;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
		return;
	}

	if(me->mind1&1)
	{
		if(me->dx>0)
			me->mind3=64+Random(128);
		else
			me->mind3=192+Random(128);
	}
	if(me->mind1&2)
	{
		if(me->dy>0)
			me->mind3=128+Random(128);
		else
			me->mind3=Random(128);
	}
	me->mind3=TurnToward(me->mind3,AngleFrom(me->x,me->y,goodguy->x,goodguy->y),4);

	me->facing=me->mind3/32;
	me->dx=Cosine(me->mind3)*5;
	me->dy=Sine(me->mind3)*5;
	me->mind1=0;

	if(me->seq==ANIM_MOVE)
		me->frmAdvance=128;

	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=256;
	}
}

void AI_Bobber(Guy *me,Map *map,world_t *world,Guy *badguy)
{
	if(me->mapx>=map->width || me->mapy>=map->height)
	{
		MakeNormalSound(SND_TURRETBZZT);
		me->type=MONS_NONE;
	}
	if(player.levelNum!=1 && (map->map[me->mapx+me->mapy*map->width].wall || map->map[me->mapx+me->mapy*map->width].item))
	{
		MakeNormalSound(SND_TURRETBZZT);
		me->type=MONS_NONE;
	}

	LightningBolt(me->x,me->y-me->z-FIXAMT*5,goodguy->x+Cosine(goodguy->facing*32)*40,goodguy->y+Sine(goodguy->facing*32)*40-FIXAMT*20);
	// position me way out there
	me->x=goodguy->x+Cosine(goodguy->facing*32);//*player.journal[51];
	me->y=goodguy->y+Sine(goodguy->facing*32);//*player.journal[51];
	me->mapx=(me->x>>FIXSHIFT)/TILE_WIDTH;
	me->mapy=(me->y>>FIXSHIFT)/TILE_HEIGHT;

	me->dz+=FIXAMT*14/8;	// fall SLOWLY!

	if(me->z<FIXAMT && !(world->terrain[map->map[me->mapx+me->mapy*map->width].floor].flags&(TF_WATER|TF_MUD)))
	{
		MakeNormalSound(SND_TURRETBZZT);
		me->type=MONS_NONE;
	}

	if(me->z<FIXAMT*2 && me->type)
	{
		if(me->mind==0)
		{
			me->mind=1;
			ExplodeParticles(PART_WATER,me->x,me->y,0,5);
			//MakeSound(SND_BOBSPLASH,me->x,me->y,SND_CUTOFF,500);
		}
		me->dz/=2;
		//player.journal[52]=0;
		/*if(player.journal[51]>40)
			player.journal[51]--;
		else
		{
			player.journal[50]=0;
		}*/

		/*if(player.journal[53]==0 && ShouldBob())
		{
			MakeNormalSound(SND_BOBHIT);
			ExplodeParticles(PART_WATER,me->x,me->y,0,10);
			player.journal[53]=20;
			me->dz=FIXAMT*2;
			map->BrightTorch(me->mapx,me->mapy,31,1);
		}*/
	}

	/*if(player.journal[50]==0)
	{
		me->type=MONS_NONE;
	}*/

	if(me->type==MONS_NONE)
	{
		//player.journal[50]=0;
		player.wpnReload=20;
	}
}

void AI_Swampdog(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte damageValues[] = {1,2,3,4};
	int speedValues[] = {1*FIXAMT,2*FIXAMT,1*FIXAMT,2*FIXAMT};
	int x,y,z;


	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_SWAMPOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_SWAMPDIE,me->x,me->y,SND_CUTOFF,1200);
	}
    
	if(me->mind)
		me->mind--;

	if(me->aiType==MONS_SWAMPDOG4 && me->seq==ANIM_DIE && Random(10)==0)
	{
		y=me->y-FIXAMT*20+Random(FIXAMT*41);
		x=me->x-32*FIXAMT+Random(65*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,damageValues[me->aiType-MONS_SWAMPDOG],map,world);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A3 && me->frm==6 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;

			if(me->aiType==MONS_SWAMPDOG3 || me->aiType==MONS_SWAMPDOG4)
			{
				if(me->mind3&1)
				{
					z=(me->facing+8)*32-32;
					z+=(me->mind3-1)*8;
					z=z&255;
				}
				else
				{
					z=(me->facing+8)*32+32;
					z-=(me->mind3-1)*8;
					z=z&255;
				}
				if(me->aiType==MONS_SWAMPDOG4)
					FireBullet(x,y,z,BLT_BIGSHOT, me->friendly);
				else
					FireBullet(x,y,z,BLT_ENERGY, me->friendly);
				if(me->mind3>1)
				{
					// repeat fire
					me->mind3-=2;
					me->frm=4;
				}
				me->reload=3;
			}
			else
			{
				FireBullet(x,y,me->facing*32,BLT_ENERGY, me->friendly);
				me->reload=5;
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind2==0)	// unalert
	{
		if(RangeToTarget(me,goodguy)<128*FIXAMT || me->ouch>0)
		{
			// seen goodguy/got shot - ALERT!!
			me->mind2=1;
			me->action=ACTION_BUSY;
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=256;
			me->mind=30*5;
			MakeSound(SND_SWAMPALERT,me->x,me->y,SND_CUTOFF,1200);
		}
		else
		{
			if(Random(64)==0)
				me->facing=(me->facing+7+(byte)Random(3))&7;

			if(me->mind1==0)	// standing
			{
				me->dx=0;
				me->dy=0;
				if(me->seq!=ANIM_IDLE)
				{
					me->seq=ANIM_IDLE;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=64;
				}
				if(me->mind==0)
				{
					// plod around a bit
					me->mind=(byte)Random(30*4);
					me->mind1=1;
				}
			}
			else	// plodding
			{
				me->dx=Cosine(me->facing*32)*speedValues[me->aiType-MONS_SWAMPDOG]/FIXAMT;
				me->dy=Sine(me->facing*32)*speedValues[me->aiType-MONS_SWAMPDOG]/FIXAMT;
				if(me->seq!=ANIM_MOVE)
				{
					me->seq=ANIM_MOVE;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=64;
				}
				if(me->mind==0)
				{
					// stand around a bit
					me->mind=(byte)Random(30*2);
					me->mind1=0;
				}
			}
		}
	}
	else // ALERT
	{
		if(me->ouch)
			me->mind=30*5;	// force alertness when injured

		if(me->mind==0)
		{
			// time to be bored again
			DoMove(me,ANIM_A4,64,1,0,0);
			me->mind2=0;
		}
		if(me->seq==ANIM_A5 && me->frm>1)
			me->frm=0;

		if(me->seq==ANIM_IDLE)
		{
			// can't stand still, must charge
			FaceGoodguy(me,goodguy);
			if(me->aiType!=MONS_SWAMPDOG3)
			{
				MakeSound(SND_SWAMPCHARGE,me->x,me->y,SND_CUTOFF,1200);
				DoMove(me,ANIM_A2,128,0,Cosine(me->facing*32)*5,Sine(me->facing*32)*5);
			}
			else	// Shambling Thing does stand still
				DoMove(me,ANIM_A5,128,0,0,0);
		}
		if(goodguy && RangeToTarget(me,goodguy)<(72*FIXAMT) && Random(8)==0)// &&
			//map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			// get him!
			FaceGoodguy(me,goodguy);
			MakeSound(SND_SWAMPJAB,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_ATTACK,128,1,0,0);
			me->reload=0;
			return;
		}
		if(me->aiType==MONS_SWAMPDOG3)
			x=8;
		else
			x=32;
		if(goodguy && RangeToTarget(me,goodguy)<(380*FIXAMT) && Random(x)==0)// &&
			//map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			// get him!
			FaceGoodguy(me,goodguy);
			MakeSound(SND_SWAMPSHOOT,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_A3,128,1,0,0);
			me->reload=0;
			me->mind3=4*2+(byte)Random(2);

			return;
		}
	}
	if(me->seq==ANIM_A2)
	{
		// charging
		if(me->AttackCheck(32,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
		{
			goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,damageValues[me->aiType-MONS_SWAMPDOG],map,world);
		}
	}
}

void AI_LL_Bonehead(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte damageValues[] = {1,2,2,3,5};
	int speedValues[] = {4*FIXAMT,4*FIXAMT,5*FIXAMT,3*FIXAMT,2*FIXAMT};
	int x=0,y=0,i;

	// Gangrenes glow
	if(me->aiType==MONS_BONEHEAD4)
		map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,(me->y/TILE_HEIGHT)>>FIXSHIFT,12,16);

	BasicAI(me,SND_LL_SKELOUCH,SND_LL_SKELDIE,map,world,goodguy);

	if(me->aiType==MONS_BONEHEAD6 && me->seq==ANIM_DIE && Random(10)==0)
	{
		y=me->y-FIXAMT*10+Random(FIXAMT*11);
		x=me->x-32*FIXAMT+Random(65*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->frm==8)
			me->frmAdvance=256;
		if(me->seq==ANIM_ATTACK && me->aiType==MONS_BONEHEAD6)
		{
			me->dx+=Cosine(me->facing*32)*2;
			me->dy+=Sine(me->facing*32)*2;
			if(me->AttackCheck(24,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,damageValues[me->aiType-MONS_BONEHEAD2],map,world);
		}
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,damageValues[me->aiType-MONS_BONEHEAD2],map,world);
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frm==3 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->aiType==MONS_BONEHEAD3)
				FireBullet(x,y,me->facing*32,BLT_ENERGY,me->friendly);
			else if(me->aiType==MONS_BONEHEAD4)
				FireBullet(x,y,me->facing*32,BLT_GASBLAST,me->friendly);
			else if(me->aiType==MONS_BONEHEAD6)
			{
				FireBullet(x,y,(byte)((me->facing*32-16+Random(32))&255),BLT_FLAME,me->friendly);
				FireBullet(x,y,(byte)((me->facing*32-16+Random(32))&255),BLT_FLAME,me->friendly);
				FireBullet(x,y,(byte)((me->facing*32-16+Random(32))&255),BLT_FLAME,me->friendly);
			}
			me->reload=5;
			me->mind1=1;
		}
		if(me->seq==ANIM_A2 && me->aiType==MONS_BONEHEAD5 && me->reload==0 && me->frm>=2)
		{
			i=Random(TILE_WIDTH)+TILE_WIDTH*(me->frm-1);
			x=me->x+Cosine(me->facing*32)*i;
			y=me->y+Sine(me->facing*32)*i;
			FireBullet(x,y,me->facing*32,BLT_FLAMEWALL,me->friendly);
			me->reload=3;
		}
		if(me->seq==ANIM_A4 && me->frm==4 && me->reload==0)
		{
			// raise the dying
			me->reload=5;
			RaiseSkellies();
		}
		return;	// can't do nothin' right now
	}

	// randomly decide to point at Bouapha to unnerve him
	// (but only if in pursuit mode, because otherwise you'd point the wrong way)
	// Numbskulls don't do this, since they have a much nastier point they use
	// neither does Agent Skully
	if((!Random(100)) && me->mind==0 && me->aiType!=MONS_BONEHEAD5 && me->aiType!=MONS_BONEHEAD6)
	{
		me->seq=ANIM_A2;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=64;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		return;
	}

	// if you aren't wandering and you are a Bonehead or Numbskull, then kick as needed
	if(me->mind==0 && (me->aiType==MONS_BONEHEAD2 || me->aiType==MONS_BONEHEAD5))
	{
		if(goodguy && RangeToTarget(me,goodguy)<(48*FIXAMT) && Random(8)==0)// &&
			//map->CheckLOS(me->mapx,me->mapy,4,goodguy->mapx,goodguy->mapy))
		{
			// get him!
			MakeSound(SND_LL_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
	}
	if(me->mind==0 && me->aiType==MONS_BONEHEAD6)
	{
		if(goodguy && RangeToTarget(me,goodguy)<(200*FIXAMT) && Random(8)==0)// &&
			//map->CheckLOS(me->mapx,me->mapy,10,goodguy->mapx,goodguy->mapy))
		{
			// get him!
			MakeSound(SND_LL_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
	}
	// Redbones and Gangrenes shoot only
	if(me->aiType==MONS_BONEHEAD3 || me->aiType==MONS_BONEHEAD4 || me->aiType==MONS_BONEHEAD6)
	{
		if(goodguy && RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(32)==0)// &&
			//map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			// spit at him
			MakeSound(SND_LL_SKELSHOOT,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			FaceGoodguy(me,goodguy);
			return;
		}
	}
	// numbskull special attacks
	if(me->aiType==MONS_BONEHEAD5)
	{
		// resurrect any dying skellies
		if(Random(8)==0 && DyingSkellies())
		{
			// resurrect dying skellies
			MakeSound(SND_SKELSUMMON,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A4;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=64;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			FaceGoodguy(me,goodguy);
			return;
		}
		if(Random(32)==0)// &&
			//map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			// point to cause flame geyser
			MakeSound(SND_SKELBLAST,me->x,me->y,SND_CUTOFF,1200);
			FaceGoodguy(me,goodguy);
			me->seq=ANIM_A2;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=32;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			return;
		}
	}
	WanderAI(me,64,40,3,map,world,goodguy,speedValues[me->aiType-MONS_BONEHEAD2]);
}

/*void AI_LL_Frog(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte damageValues[] = {1,1,1,1};
	int speedValues[] = {3*FIXAMT,3*FIXAMT,4*FIXAMT,4*FIXAMT};
	int x,y;

	BasicAI(me,SND_FROGOUCH,SND_FROGDIE,map,world,goodguy);

	if(me->type==MONS_FROG6 && Random(3)==0)
	{
		x=me->x-10*FIXAMT+Random(21*FIXAMT);
		y=me->y-10*FIXAMT+Random(21*FIXAMT);
		FireBullet(x,y,0,BLT_FLAMEWALL,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0 && me->type==MONS_FROG3)
		{
			x=me->x+Cosine(me->facing*32)*30;
			y=me->y+Sine(me->facing*32)*30;
			if(me->AttackCheck(10,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,damageValues[me->type-MONS_FROG3],map,world);
			}
			x=me->x+Cosine(me->facing*32)*60;
			y=me->y+Sine(me->facing*32)*60;
			if(me->AttackCheck(10,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,damageValues[me->type-MONS_FROG3],map,world);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0 && me->type==MONS_FROG4)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			FireBullet(x,y,me->facing*32,BLT_GASBLAST,me->friendly);
			me->reload=5;
		}
		if(me->seq==ANIM_ATTACK && me->frm==2 && me->reload==0 && me->type==MONS_FROG5)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			FireBullet(x,y,me->facing*32,BLT_ENERGY,me->friendly);
			me->reload=30;
		}
		if(me->seq==ANIM_DIE && me->type==MONS_FROG4 && me->frm==4)
		{
			for(y=0;y<4;y++)
			{
				x=Random(256);
				FireBullet(me->x,me->y,x,BLT_GASBLAST,me->friendly);
			}
		}
		if(me->seq==ANIM_DIE && me->type==MONS_FROG5 && me->frm==4 && me->reload==0)
		{
			FireBullet(me->x,me->y,0,BLT_FLAMEWALL,me->friendly);
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0 && me->type!=MONS_FROG6)	// heading for Bouapha
	{
		if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(8)==0 && me->type==MONS_FROG3)// &&
			//map->CheckLOS(me->mapx,me->mapy,5,goodguy->mapx,goodguy->mapy))
		{
			// get him!
			MakeSound(SND_FROGLICK,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
	}

	if(me->type==MONS_FROG4 && RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(64)==0)// &&
		//map->CheckLOS(me->mapx,me->mapy,8,goodguy->mapx,goodguy->mapy))
	{
		// spit out gas
		MakeSound(SND_FROGLICK,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}
	if(me->type==MONS_FROG5 && RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(64)==0 && me->reload==0)// &&
		//map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	{
		// spit out fireball
		MakeSound(SND_FROGLICK,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=256;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}

	WanderAI(me,64,40,3,map,world,goodguy,speedValues[me->type-MONS_FROG3]);
}*/

void AI_FireFrog(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	BasicAI(me,SND_FROGOUCH,SND_FROGDIE,map,world,goodguy);

	if(Random(3)==0)
	{
		x=me->x-10*FIXAMT+Random(21*FIXAMT);
		y=me->y-10*FIXAMT+Random(21*FIXAMT);
		FireBullet(x,y,0,BLT_FLAMEWALL,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		return;	// can't do nothin' right now
	}

	WanderAI(me,64,40,3,map,world,goodguy,4*FIXAMT);
}

void AI_PoisonFrog(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	BasicAI(me,SND_FROGOUCH,SND_FROGDIE,map,world,goodguy);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			FireBullet(x,y,me->facing*32,BLT_GASBLAST,me->friendly);
			me->reload=5;
		}
		if(me->seq==ANIM_DIE && me->frm==4)
		{
			for(y=0;y<4;y++)
			{
				x=Random(256);
				FireBullet(me->x,me->y,x,BLT_GASBLAST,me->friendly);
			}
		}
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(64)==0)// &&
		//map->CheckLOS(me->mapx,me->mapy,8,goodguy->mapx,goodguy->mapy))
	{
		// spit out gas
		MakeSound(SND_FROGLICK,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}

	WanderAI(me,64,40,3,map,world,goodguy,3*FIXAMT);
}

void AI_Toad(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	BasicAI(me,SND_FROGOUCH,SND_FROGDIE,map,world,goodguy);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==2 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			FireBullet(x,y,me->facing*32,BLT_ENERGY,me->friendly);
			me->reload=30;
		}
		if(me->seq==ANIM_DIE && me->frm==4 && me->reload==0)
		{
			FireBullet(me->x,me->y,0,BLT_FLAMEWALL,me->friendly);
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(8)==0)// &&
			//map->CheckLOS(me->mapx,me->mapy,5,goodguy->mapx,goodguy->mapy))
	{
		// get him!
		MakeSound(SND_FROGLICK,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}

	if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(64)==0 && me->reload==0)// &&
		//map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	{
		// spit out fireball
		MakeSound(SND_FROGLICK,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=256;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}

	WanderAI(me,64,40,3,map,world,goodguy,4*FIXAMT);
}

//#include "log.h"

void AI_LL_Zombie(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte damageValues[] = {2,3,4,4};
	int speedValues[] = {2*FIXAMT,4*FIXAMT,1*FIXAMT,2*FIXAMT};
	int x,y,i;
	Guy *g;

	BasicAI(me,SND_LL_ZOMBIEOUCH,SND_LL_ZOMBIEDIE,map,world,goodguy);

	if(me->seq==ANIM_DIE && me->aiType==MONS_ZOMBIE5)
	{
		if(Random(5)==0)
		{
			FireBullet(me->x-32*FIXAMT+Random(65)*FIXAMT,me->y-32*FIXAMT+Random(65)*FIXAMT,0,BLT_BOOM,me->friendly);
			FireBullet(me->x-32*FIXAMT+Random(65)*FIXAMT,me->y-32*FIXAMT+Random(65)*FIXAMT,0,BLT_EVILFACE,me->friendly);
		}
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5 && me->reload==0 && goodguy)
		{
			me->reload=5;
			me->dx=Cosine(me->facing*32)*8;
			me->dy=Sine(me->facing*32)*8;
			me->reload=5;
		}
		if(me->seq==ANIM_ATTACK && me->frm>5)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(24,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*12,Sine(me->facing*32)*12,damageValues[me->aiType-MONS_ZOMBIE2],map,world);
			}
		}
		if(me->seq==ANIM_ATTACK && me->frm>9)
		{
			Dampen(&me->dx,FIXAMT*2);
			Dampen(&me->dy,FIXAMT*2);
			if(me->frm==11 && me->reload==0 && (me->aiType==MONS_ZOMBIE3 || me->aiType==MONS_ZOMBIE5))	// bombie
			{
				// blow self up
				FireBullet(me->x,me->y,0,BLT_MEGABOOM,me->friendly);
				if(me->aiType==MONS_ZOMBIE3)
				{
					me->seq=ANIM_DIE;
					me->frm=0;
					me->frmTimer=0;
					me->frmAdvance=128;
					me->dx=0;
					me->dy=0;
					me->hp=0;
				}
				return;
			}
		}
		if(me->seq==ANIM_A1 && me->frm==3 && me->reload==0 && goodguy)
		{
			if(me->aiType==MONS_ZOMBIE4)
			{
				x=me->x+Cosine(me->facing*32)*16;
				y=me->y+Sine(me->facing*32)*16;
				FireBullet(x,y,me->facing*32,BLT_EARTHSPIKE,me->friendly);
				FireBullet(x,y,((me->facing+6)*32)&255,BLT_EARTHSPIKE,me->friendly);
				FireBullet(x,y,((me->facing+1)*32)&255,BLT_EARTHSPIKE,me->friendly);
				me->reload=5;
				me->mind1=1;
			}
			else
			{
				i=0;
				while(i<20)
				{
					x=me->x-48+Random(97)*FIXAMT;
					y=me->y-48+Random(97)*FIXAMT;
					g=AddGuy(x,y,FIXAMT*40,MONS_ZOMBIE3,me->friendly);
					if(g && !g->CanWalk(g->x,g->y,map,world))
					{
						g->type=MONS_NONE;
						i++;
						continue;
					}
					else
					{
						// MakeRingParticle(x,y,FIXAMT*40,20,40);
						FireBullet(x,y,0,BLT_BOOM,me->friendly);
						break;
					}
				}
				me->reload=5;
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)	// heading for Bouapha
	{
		x=128*FIXAMT;
		y=32;
		if(me->aiType==MONS_ZOMBIE3)
			y=16;

		if(RangeToTarget(me,goodguy)<(x) && Random(y)==0)// &&
			//map->CheckLOS(me->mapx,me->mapy,8,goodguy->mapx,goodguy->mapy))
		{
			// get him!
			MakeSound(SND_LL_ZOMBIELEAP,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			return;
		}
	}
	else	// wandering
	{
		if(me->aiType==MONS_ZOMBIE3)	// bombie
		{
			me->dx=me->dx/2;
			me->dy=me->dy/2;
		}
	}

	if((me->aiType==MONS_ZOMBIE4 || me->aiType==MONS_ZOMBIE5) && RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(64)==0)
	{
		// stomp earth at him
		MakeSound(SND_ZOMBIESTOMP,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}
	if(me->seq==ANIM_MOVE)
	{
		switch(me->aiType)
		{
			case MONS_ZOMBIE2:
				me->frmAdvance=64;
				break;
			case MONS_ZOMBIE3:	// bombie
				me->frmAdvance=128;
				break;
			case MONS_ZOMBIE4:	// zombie lord
			case MONS_ZOMBIE5:
				me->frmAdvance=48;
				break;
		}
	}
	if(me->aiType!=MONS_ZOMBIE3)
		WanderAI(me,64,40,3,map,world,goodguy,speedValues[me->aiType-MONS_ZOMBIE2]);
	else
		WanderAI(me,128,10,3,map,world,goodguy,speedValues[me->aiType-MONS_ZOMBIE2]);
}

void AI_LL_Bat(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte damageValues[] = {1,1,2,1};
	int speedValues[] = {8*FIXAMT,5*FIXAMT,6*FIXAMT,5*FIXAMT};
	int x,y;
	//bullet_t *b;

	BasicAI(me,SND_LL_BATOUCH,SND_LL_BATDIE,map,world,goodguy);

	if(me->aiType==MONS_BAT5 && Random(3)==0)
	{
		FireBullet(me->x,me->y,(byte)Random(256),BLT_GASBLAST,me->friendly);
		// if(b)
		// {
		// 	b->dx/=2;
		// 	b->dy/=2;
		// }
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==2 && me->reload==0 && goodguy)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,damageValues[me->aiType-MONS_BAT2],map,world);
			me->reload=2;

		}
		if(me->seq==ANIM_A1 && me->frm>1 && goodguy && me->hp>0)
		{
			// diving attack hit check
			x=me->x;
			y=me->y;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,damageValues[me->aiType-MONS_BAT2],map,world);
				me->seq=ANIM_A2;	// bounce off
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=64;
				me->dx=-me->dx/4;
				me->dy=-me->dy/4;
			}
		}
		if(me->seq==ANIM_A3 && me->frm==3 && me->reload==0)
		{
			FireBullet(me->x,me->y,me->facing*32,BLT_ENERGY,me->friendly);
			me->reload=5;
		}
		if(me->seq==ANIM_DIE)
			me->frmAdvance=256;

		return;	// can't do nothin' right now
	}

	if(me->aiType!=MONS_BAT5)
	{
		if(RangeToTarget(me,goodguy)<(48*FIXAMT) && Random(4)==0)// &&
			//map->CheckLOS(me->mapx,me->mapy,6,goodguy->mapx,goodguy->mapy))
		{
			// get him!
			MakeSound(SND_LL_BATEYES,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			me->reload=0;
			FaceGoodguy(me,goodguy);
			return;
		}

		if(RangeToTarget(me,goodguy)>(75*FIXAMT) && Random(32)==0 && me->aiType==MONS_BAT3)// &&
			//map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			FaceGoodguy(me,goodguy);
			MakeSound(SND_LL_BATDIVE,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A1;	// diving attack move
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=Cosine(me->facing*32)*12;
			me->dy=Sine(me->facing*32)*12;
			return;
		}

		if(RangeToTarget(me,goodguy)>(200*FIXAMT) && Random(32)==0 && me->aiType==MONS_BAT4)// &&
			//map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			MakeSound(SND_BATSHOOT,me->x,me->y,SND_CUTOFF,1200);
			me->seq=ANIM_A3;	// shoot
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			me->dx=0;
			me->dy=0;
			FaceGoodguy(me,goodguy);
			return;
		}
	}

	WanderAI(me,64,40,3,map,world,goodguy,speedValues[me->aiType-MONS_BAT2]);
}

void AI_Mummy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte damageValues[] = {1,1,2,3,5};
	int speedValues[] = {4*FIXAMT,1*FIXAMT,2*FIXAMT,1*FIXAMT,2*FIXAMT};
	int x,y,z;
	byte ang;
	Guy *g;

	BasicAI(me,SND_LL_ZOMBIEOUCH,SND_LL_ZOMBIEDIE,map,world,goodguy);

	if(me->aiType==MONS_MUMMY5 && me->seq==ANIM_DIE && Random(5)==0)
	{
		y=me->y-FIXAMT*10+Random(FIXAMT*11);
		x=me->x-32*FIXAMT+Random(65*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,damageValues[me->aiType-MONS_MUMMY],map,world);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frm==3 && me->reload==0 && me->aiType!=MONS_MUMMY3)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			MakeSound(SND_MUMMYSHOOT,me->x,me->y,SND_CUTOFF,1200);
			if(me->aiType!=MONS_MUMMY5)
			{
				FireBullet(x,y,me->facing*32,BLT_ENERGY,me->friendly);
				if(me->aiType==MONS_MUMMY4)
				{
					FireBullet(x,y,(me->facing*32+240)&255,BLT_ENERGY,me->friendly);
					FireBullet(x,y,(me->facing*32+16)&255,BLT_ENERGY,me->friendly);
				}
			}
			else
			{
				ang=AngleFrom(me->x,me->y,goodguy->x,goodguy->y);
				FireExactBullet(me->x,me->y,0,Cosine(ang)*6,Sine(ang)*6,0,0,60,(byte)ang,BLT_WIND,me->friendly);
				//FireBullet(x,y,me->facing*32,BLT_WIND,me->friendly);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frm>=3 && me->frm<=5 && me->reload==0 && me->aiType==MONS_MUMMY3)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			MakeSound(SND_MUMMYSHOOT,me->x,me->y,SND_CUTOFF,1200);
			z=me->facing*32-12+Random(25)+256;
			FireBullet(x,y,z&255,BLT_ENERGY,me->friendly);
		}
		if(me->seq==ANIM_A2 && me->frm==4 && me->reload==0)
		{
			g=AddGuy(me->x-FIXAMT*50,me->y+FIXAMT*10,0,MONS_MUMMY+2*(me->aiType==MONS_MUMMY5),me->friendly);
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				g->type=MONS_NONE;	// vanish if this spot is occupied
			g=AddGuy(me->x+FIXAMT*50,me->y+FIXAMT*10,0,MONS_MUMMY+2*(me->aiType==MONS_MUMMY5),me->friendly);
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				g->type=MONS_NONE;	// vanish if this spot is occupied
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	if(me->mind1==0)
		me->mind1=70;
	else
	{
		me->mind1--;
		if(me->mind1==0)
		{
			if((me->aiType==MONS_MUMMY4 || me->aiType==MONS_MUMMY5) && Random(4)==0)
			{
				MakeSound(SND_MUMMYSUMMON,me->x,me->y,SND_CUTOFF,1200);
				me->seq=ANIM_A2;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
				me->action=ACTION_BUSY;
				me->dx=0;
				me->dy=0;
				me->reload=0;
			}
		}
	}

	if(RangeToTarget(me,goodguy)<(72*FIXAMT) && Random(8)==0)// &&
		//map->CheckLOS(me->mapx,me->mapy,4,goodguy->mapx,goodguy->mapy))
	{
		// get him!
		MakeSound(SND_MUMMYSMACK,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		if(me->aiType==MONS_MUMMY)
			me->frmAdvance=256;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}

	x=64;
	if(me->aiType==MONS_MUMMY3)
		x=32;
	if(me->aiType!=MONS_MUMMY && RangeToTarget(me,goodguy)<(360*FIXAMT) && Random(x)==0)// &&
		//map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	{
		// shoot
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}

	if(me->aiType==MONS_MUMMY && me->seq==ANIM_MOVE)
		me->frmAdvance=256;

	WanderAI(me,1024,2,1,map,world,goodguy,speedValues[me->aiType-MONS_MUMMY]);
}

void AI_EvilTree(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;

	// initially adjust position
	if(me->mind3==0)
	{
		me->x-=FIXAMT*3;
		me->y+=FIXAMT;
		me->mind3=1;
	}

	BasicAI(me,SND_TREEOUCH,SND_TREEDIE,map,world,goodguy);

	if(me->aiType==MONS_EVILTREE3 && me->seq==ANIM_DIE && Random(10)==0)
	{
		y=me->y-FIXAMT*10+Random(FIXAMT*11);
		x=me->x-32*FIXAMT+Random(65*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0)
		{
			if(me->aiType==MONS_EVILTREE2)
			{
				// gas attack
				MakeSound(SND_SKELGAS,me->x,me->y,SND_CUTOFF,1200);
				FireBullet(me->x,me->y,64,BLT_GASBLAST,me->friendly);
				FireBullet(me->x,me->y,64+16,BLT_GASBLAST,me->friendly);
				FireBullet(me->x,me->y,64-16,BLT_GASBLAST,me->friendly);
				me->reload=30*2;
			}
			else if(me->aiType==MONS_EVILTREE)
			{
				// summon frog
				MakeSound(SND_TREEFROG,me->x,me->y,SND_CUTOFF,1200);
				x=me->x;
				y=me->y+40*FIXAMT;
				g=AddGuy(x,y,FIXAMT*10,MONS_FROG3,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				me->reload=30*3;
			}
			else
			{
				// summon frog
				MakeSound(SND_TREEFROG,me->x,me->y,SND_CUTOFF,1200);
				x=me->x;
				y=me->y+40*FIXAMT;
				g=AddGuy(x,y,FIXAMT*10,MONS_FROG4,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				me->reload=30*3;
			}
		}
		// if(me->seq==ANIM_DIE && me->frm==0 && me->frmTimer<64)
		// {
		// 	if(player.worldNum==WORLD_NORMAL || player.worldNum==WORLD_REMIX)
		// 		map->map[me->mapx+me->mapy*map->width].item=ITM_TREE2;
		// }
		return;	// can't do nothin' right now
	}


	if(me->aiType==MONS_EVILTREE)
	{
		// launch frogs when player is very near
		if(RangeToTarget(me,goodguy)<(256*FIXAMT) && me->reload==0 && Random(16)==0)
		{
			// shoot
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			return;
		}
	}
	else if(me->aiType==MONS_EVILTREE3)
	{
		// launch frogs when player is near
		if(RangeToTarget(me,goodguy)<(400*FIXAMT) && me->reload==0 && Random(16)==0)
		{
			// shoot
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			return;
		}
	}
	else
	{
		// launch gas at a standard rate
		if(me->reload==0)
		{
			// shoot
			me->seq=ANIM_ATTACK;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
			me->action=ACTION_BUSY;
			return;
		}
	}
}

void AI_LL_Ghost(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte damageValues[] = {1,1,2};
	int speedValues[] = {3*FIXAMT,5*FIXAMT,4*FIXAMT};
	int x,y;

	// banshees glow
	if(me->aiType==MONS_GHOST4)
		map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
						 (me->y/TILE_HEIGHT)>>FIXSHIFT,8,9);
	// shades make darkness
	else if(me->aiType==MONS_GHOST3)
		map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
						 (me->y/TILE_HEIGHT)>>FIXSHIFT,-4,8);

	BasicAI(me,SND_LL_GHOSTOUCH,SND_LL_GHOSTDIE,map,world,goodguy);

	if(me->mind3>0)
		me->mind3--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(24,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*2,Sine(me->facing*32)*2,damageValues[me->aiType-MONS_GHOST2],map,world);
			}
		}
		if(me->seq==ANIM_A1 && me->frm==4 && me->reload==0)
		{
			// banshees teleport randomly
			if(me->aiType==MONS_GHOST4)
			{
				me->x=(me->x-320*FIXAMT)+Random(640*FIXAMT);
				me->y=(me->y-240*FIXAMT)+Random(480*FIXAMT);
				if(me->x<32*FIXAMT)
					me->x=32*FIXAMT;
				if(me->y<32*FIXAMT)
					me->y=32*FIXAMT;
				if(me->x>map->width*TILE_WIDTH*FIXAMT-TILE_WIDTH*FIXAMT)
					me->x=map->width*TILE_WIDTH*FIXAMT-TILE_WIDTH*FIXAMT;
				if(me->y>map->height*TILE_HEIGHT*FIXAMT-TILE_HEIGHT*FIXAMT)
					me->y=map->height*TILE_HEIGHT*FIXAMT-TILE_HEIGHT*FIXAMT;
			}
			else	// shades teleport to player
			{
				me->x=Random(80*FIXAMT)+40*FIXAMT;
				if(Random(2)==0)
					me->x=-me->x;
				me->x+=goodguy->x;
				me->y=Random(60*FIXAMT)+30*FIXAMT;
				if(Random(2)==0)
					me->y=-me->y;
				me->y+=goodguy->y;
				if(me->x<32*FIXAMT)
					me->x=32*FIXAMT;
				if(me->y<32*FIXAMT)
					me->y=32*FIXAMT;
				if(me->x>map->width*TILE_WIDTH*FIXAMT-TILE_WIDTH*FIXAMT)
					me->x=map->width*TILE_WIDTH*FIXAMT-TILE_WIDTH*FIXAMT;
				if(me->y>map->height*TILE_HEIGHT*FIXAMT-TILE_HEIGHT*FIXAMT)
					me->y=map->height*TILE_HEIGHT*FIXAMT-TILE_HEIGHT*FIXAMT;
			}
			for(x=0;x<4;x++)
			{
				map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,16,9);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && (me->frm==2 || me->frm==8 || me->frm==14))
		{
			MakeSound(SND_GHOSTSPIN,me->x,me->y,SND_CUTOFF,1200);
			if(me->frm==14)
			{
				me->dx=Cosine(me->facing*32)*6;
				me->dy=Sine(me->facing*32)*6;
			}
		}
		if(me->seq==ANIM_A2 && me->frm==18)
			MakeSound(SND_GHOSTPUNCH,me->x,me->y,SND_CUTOFF,1200);
		if(me->seq==ANIM_A2 && me->frm>=17 && me->frm<=25)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(24,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,damageValues[me->aiType-MONS_GHOST2]*3,map,world);
			}
		}
		if(me->seq==ANIM_A2 && me->frm>21)
		{
			Dampen(&me->dx,FIXAMT/4);
			Dampen(&me->dy,FIXAMT/4);
		}
		if(me->seq==ANIM_A3 && me->frm==2 && me->reload==0)
		{
			// scream
			me->reload=10;

			// MakeRingParticle(me->x,me->y,0,180,40);
			if(RangeToTarget(me,goodguy)<200*FIXAMT)
			{
				goodguy->dx=0;
				goodguy->dy=0;
				goodguy->frozen=30*2;
				MakeSound(SND_FREEZE,x*FIXAMT,y*FIXAMT,SND_CUTOFF,1200);
			}
		}
		return;	// can't do nothin' right now
	}

	// punch
	// Shades only use super punches if player is freeze
	if(RangeToTarget(me,goodguy)<(88*FIXAMT) && Random(6)==0 &&
		(me->aiType!=MONS_GHOST3 || goodguy->frozen==0))
	{
		// get him!
		MakeSound(SND_GHOSTPUNCH,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,164,1,0,0);
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}
	// Super punch!!
	if(me->aiType==MONS_GHOST3 && RangeToTarget(me,goodguy)<(160*FIXAMT) && goodguy->frozen &&
		Random(32)==0)
	{
		DoMove(me,ANIM_A2,256,1,0,0);
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}
	// teleport if non-Ghost
	if(me->aiType!=MONS_GHOST2 && RangeToTarget(me,goodguy)>(320*FIXAMT) && Random(32)==0)
	{
		MakeSound(SND_GHOSTTELEPORT,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A1,128,1,0,0);
		me->reload=0;
		return;
	}
	// Banshees scream
	if(me->aiType==MONS_GHOST4 && RangeToTarget(me,goodguy)<256*FIXAMT && me->mind3==0 && Random(128)==0)
	{
		// scream
		me->mind3=30*5;
		MakeSound(SND_GHOSTSCREAM,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A3,128,1,0,0);
		me->reload=0;
		return;
	}
	WanderAI(me,256,20,3,map,world,goodguy,speedValues[me->aiType-MONS_GHOST2]);
}

void AI_Wolfman(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte damageValues[] = {1,1,2};
	int speedValues[] = {6*FIXAMT,4*FIXAMT,5*FIXAMT};
	int x,y;
	Guy *g;

	if(me->aiType==MONS_WOLFMAN2 && me->mind2>0)
	{
		map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
						 (me->y/TILE_HEIGHT)>>FIXSHIFT,4,2);
	}

	BasicAI(me,SND_WOLFOUCH,SND_WOLFDIE,map,world,goodguy);

	if(me->mind3 && me->mind2==0)
		me->mind3--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==5)
		{
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=10)
		{
			Dampen(&me->dx,FIXAMT);
			Dampen(&me->dy,FIXAMT);
			if(me->AttackCheck(40,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,damageValues[me->aiType-MONS_WOLFMAN],map,world);
			}
			if(me->frm==10)
			{
				me->dx=0;
				me->dy=0;
			}
		}
		if(me->seq==ANIM_A1 && me->frm==2 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;
			FireBullet(x,y,me->facing*32,BLT_ENERGY,me->friendly);
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && me->frm>=5 && me->frm<=15)
		{
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,damageValues[me->aiType-MONS_WOLFMAN],map,world);
			}
		}
		if(me->seq==ANIM_A2)
		{
			// constantly turn toward goodguy
			FaceGoodguy2(me,goodguy);
			if(me->frm<15 && me->frm>4)
			{
				x=Cosine(me->facing*32)*12;
				y=Sine(me->facing*32)*12;
			}
			else
			{
				x=0;
				y=0;
			}
			me->dx-=x;
			Dampen(&me->dx,FIXAMT+FIXAMT/2);
			me->dx+=x;
			me->dy-=y;
			Dampen(&me->dy,FIXAMT+FIXAMT/2);
			me->dy+=y;
		}
		if(me->seq==ANIM_A3 && me->frm==6 && me->reload==0)
		{
			// howl effect
			me->reload=10;
			if(me->aiType==MONS_WOLFMAN2)
			{
				// go fast for 3 seconds
				me->mind2=30*3;
			}
			else // dire wolf summons dogboys
			{
				// summon one randomly off each edge of the screen
				// left
				GetCamera(&x,&y);
				x=x-320-80-Random(320);
				y=y-240+Random(480);
				g=AddGuy(x*FIXAMT,y*FIXAMT,0,MONS_WOLFMAN,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				// right
				GetCamera(&x,&y);
				x=x+320+80+Random(320);
				y=y-240+Random(480);
				g=AddGuy(x*FIXAMT,y*FIXAMT,0,MONS_WOLFMAN,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				// top
				GetCamera(&x,&y);
				x=x-320+Random(640);
				y=y-240-80-Random(240);
				g=AddGuy(x*FIXAMT,y*FIXAMT,0,MONS_WOLFMAN,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				// bottom
				GetCamera(&x,&y);
				x=x-320+Random(640);
				y=y+240+80+Random(240);
				g=AddGuy(x*FIXAMT,y*FIXAMT,0,MONS_WOLFMAN,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
			}
		}
		if(me->seq==ANIM_DIE)
			me->frmAdvance=190;
		return;	// can't do nothin' right now
	}

	// dire wolves don't pounce
	if(me->aiType!=MONS_WOLFMAN3 && RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(12)==0)// &&
		//map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	{
		// get him!
		MakeSound(SND_WOLFPOUNCE,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=160;
		me->action=ACTION_BUSY;
		me->reload=0;
		me->dx=0;
		me->dy=0;
		return;
	}
	// dogboys don't spit
	if(me->aiType>=MONS_WOLFMAN2 && RangeToTarget(me,goodguy)<(360*FIXAMT) && Random(128)==0)// &&
		//map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	{
		// shoot
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		MakeSound(SND_WOLFSPIT,me->x,me->y,SND_CUTOFF,1200);
		FaceGoodguy(me,goodguy);
		return;
	}
	// only dire wolves charge
	if(me->aiType==MONS_WOLFMAN3 && RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(32)==0)// &&
		//map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	{
		// charge!!!
		MakeSound(SND_WOLFCHARGE,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A2;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		FaceGoodguy(me,goodguy);
		return;
	}
	// Dire Wolf and Wolfman howl
	if((me->aiType==MONS_WOLFMAN2 || me->aiType==MONS_WOLFMAN3) && me->mind3==0 && me->mind2==0)// && Random(128)==0 &&
		//map->CheckLOS(me->mapx,me->mapy,20,goodguy->mapx,goodguy->mapy))
	{
		// howl
		me->mind3=30*5;
		MakeSound(SND_WOLFHOWL,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A3;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}
	if(me->seq==ANIM_MOVE)
		me->frmAdvance=256;

	WanderAI(me,1024,2,1,map,world,goodguy,speedValues[me->aiType-MONS_WOLFMAN]);
}

void AI_StickWitch(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	//bullet_t *b;

	//SetBossHP(me->hp,MonsterHP(me->aiType));

	BasicAI(me,SND_STICKOUCH,SND_STICKDIE,map,world,goodguy);

	if(me->mind3<2)
	{
		if(me->mind3==0 && (RangeToTarget(me,goodguy)<400*FIXAMT || me->ouch))
		{
			//BeginChatting(236);
			me->mind3=1;
			return;
		}
		else if(me->mind3==1 && (RangeToTarget(me,goodguy)<400*FIXAMT || me->ouch))
		{
			me->mind3=2;
			MakeSound(SND_WITCHHI,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_A2,128,1,0,0);
		}
		return;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->reload==0)
		{
			// shoot
			switch(me->mind2)
			{
				case 0:	// blast spell
					MakeSound(SND_CATKICK,me->x,me->y,SND_CUTOFF,1200);
					x=me->x+Cosine(me->mind*32)*16;
					y=me->y+Sine(me->mind*32)*16;
					FireBullet(x,y,(me->mind*32+256-16+(byte)Random(32)),BLT_BIGSHOT,me->friendly);
					me->reload=2;
					break;
				case 1:	// gas blast
					MakeSound(SND_SKELGAS,me->x,me->y,SND_CUTOFF,1200);
					x=me->x+Cosine(me->mind*32)*16;
					y=me->y+Sine(me->mind*32)*16;
					FireBullet(x,y,(me->mind*32+256-16+(byte)Random(32)),BLT_GASBLAST,me->friendly);
					me->reload=1;
					break;
				case 2:	// flames
					MakeSound(SND_STEAM,me->x,me->y,SND_CUTOFF,1200);
					x=me->x+Cosine(me->mind*32)*16;
					y=me->y+Sine(me->mind*32)*16;
					FireBullet(x,y,(me->mind*32+256-16+(byte)Random(32)),BLT_FLAME,me->friendly);
					me->reload=1;
					break;
				case 3:	// wind missiles
					MakeSound(SND_FRANKMISSILE,me->x,me->y,SND_CUTOFF,1200);
					x=me->x+Cosine(me->mind*32)*16;
					y=me->y+Sine(me->mind*32)*16;
					FireBullet(x,y,(me->mind*32+256-16+(byte)Random(32)),BLT_WIND,me->friendly);
					me->reload=3;
					break;
				case 4: // earth spikes
					MakeSound(SND_ZOMBIESTOMP,me->x,me->y,SND_CUTOFF,1200);
					x=me->x+Cosine(me->mind*32)*16;
					y=me->y+Sine(me->mind*32)*16;
					FireBullet(x,y,(me->mind*32+256-16+(byte)Random(32)),BLT_EARTHSPIKE,me->friendly);
					me->reload=3;
					break;
			}
		}
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<512*FIXAMT && Random(16)==0)
	{
		x=me->facing;
		FaceGoodguy3(me,goodguy);
		me->mind=me->facing;
		me->facing=x;
		DoMove(me,ANIM_ATTACK,128,1,me->dx,me->dy);
		return;
	}


	if(me->mind3>2)
	{
		me->mind3--;
		if(me->mind3<10)
		{
			// MakeRingParticle(me->x,me->y,0,(15-me->mind3)*3,50);
			if(RangeToTarget(me,goodguy)<70*FIXAMT)
				goodguy->GetShot(Cosine(me->facing)*16,Sine(me->facing)*16,8,map,world);
		}
	}
	else
	{
		me->mind2=(byte)Random(5);
		me->mind3=30*2+(byte)Random(30*5);
		MakeSound(SND_STICKATTACK,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->ouch>0)
	{
		if(me->mind3>10)
			me->mind3-=5;
		DoMove(me,ANIM_A1,128,0,me->dx,me->dy);
		return;
	}

	if(!(me->mind1--))	// time to get a new direction
	{
		me->facing=(byte)Random(8);
		me->mind1=(byte)Random(30*3)+1;
	}

	me->dx=(Cosine(me->facing*32)*8*FIXAMT)/FIXAMT;
	me->dy=(Sine(me->facing*32)*8*FIXAMT)/FIXAMT;
	if(me->seq!=ANIM_MOVE)
	{
		me->seq=ANIM_MOVE;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
	}
}

void AI_Larry(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	//Guy *g;

	// if(RangeToTarget(me,goodguy)<400*FIXAMT)
	// 	SetBossHP(me->hp,MonsterHP(me->aiType));

	//Burn((me->mind2*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT,(me->mind3*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT,0);

	BasicAI(me,SND_WOLFOUCH,SND_WOLFDIE,map,world,goodguy);

	if(me->mind)
		me->mind--;

	// if(me->mind<30)
	// {
	// 	Burn(me->x,me->y,Random(50*FIXAMT));
	// }

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4)
		{
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=10)
		{
			Dampen(&me->dx,FIXAMT);
			Dampen(&me->dy,FIXAMT);
			if(me->AttackCheck(40,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,3,map,world);
			}
			if(me->frm==8 && me->frmTimer<128)
			{
				x=me->x+Cosine(me->facing*32)*48;
				y=me->y+Sine(me->facing*32)*48;
				// MakeRingParticle(x,y,0,128,64);
				if(me->AttackCheck(128,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				{
					goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,3,map,world);
				}
				ShakeScreen(15);
			}
			if(me->frm==10)
			{
				me->dx=0;
				me->dy=0;
			}
		}
		if(me->seq==ANIM_A1 && (me->frm==6 || me->frm==7) && me->frmTimer<128)
		{
			x=me->x+Cosine((me->facing+7+2*(7-me->frm))*32)*20;
			y=me->y+Sine((me->facing+7+2*(7-me->frm))*32)*20;
			MakeSound(SND_LARRYDARTS,me->x,me->y,SND_CUTOFF,1200);
			for(i=0;i<10;i++)
			{
				FireBullet(x,y,(me->facing*32+(me->frm-6)*8+256-48+i*(96/10))&255,BLT_CLAW,me->friendly);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && me->frm>=5 && me->frm<=15)
		{
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,3,map,world);
			}
		}
		if(me->seq==ANIM_DIE)
		{
		// 	if(player.worldNum==WORLD_NORMAL || player.worldNum==WORLD_REMIX)
		// 	{
			//g=AddGuy(me->x,me->y,me->z,MONS_VILLAGER2);
			//BadgeCheck(BE_KILL,me->aiType,map);
			//PlayerSetVar(VAR_QUESTDONE+QUEST_WOLF,1);
			me->aiType=MONS_HUMANLARRY;
			me->type=MONS_HUMANLARRY;
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			// if(g)
			// 	g->tag=16;
		// 	}
		}
		return;	// can't do nothin' right now
	}

	// if(map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	// {
		// if goodguy is in sight
	if(RangeToTarget(me,goodguy)<(256*FIXAMT) && Random(12)==0)
	{
		// get him!
		MakeSound(SND_LARRYPOUNCE,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,128,1,0,0);
		FaceGoodguy2(me,goodguy);
		return;
	}
	if(Random(32)==0)	// fire at any range
	{
		// shoot darts
		MakeSound(SND_LARRYPOUNCE,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A1,200,1,0,0);
		FaceGoodguy2(me,goodguy);
		return;
	}
	SelectDestination(me,goodguy,map,0);
	// 	if(me->mapx==me->mind2 && me->mapy==me->mind3)
	// 	{
	// 		// me->mind2=goodguy->mapx;
	// 		// me->mind3=goodguy->mapy;
	// 		SelectDestination(me,goodguy,map,0);
	// 	}
	// //}
	// else
	// {
	// 	if(me->mapx==me->mind2 && me->mapy==me->mind3)
	// 		SelectDestination(me,goodguy,map,0);
	// }

	/*
	if(me->mind1 && (abs(me->dx)>FIXAMT*3 || abs(me->dy)>FIXAMT*3))
	{
		me->mind1=0;
		me->dx=0;
		me->dy=0;
		SelectDestination(me,goodguy,map,1);
	}
	*/
	if(me->mind==0)
	{
		me->aiType=MONS_HUMANLARRY;
		me->type=MONS_HUMANLARRY;
		DoMove(me,ANIM_IDLE,128,0,0,0);
		MakeSound(SND_LARRYNORMAL,me->x,me->y,SND_CUTOFF,1800);
		SelectDestination(me,goodguy,map,1);
		me->mind=30*8;
		return;
	}

	if(me->mapx>me->mind2)
		me->dx-=FIXAMT;
	else if(me->mapx<me->mind2)
		me->dx+=FIXAMT;
	if(me->mapy>me->mind3)
		me->dy-=FIXAMT;
	else if(me->mapy<me->mind3)
		me->dy+=FIXAMT;
	Dampen(&me->dx,FIXAMT/4);
	Dampen(&me->dy,FIXAMT/4);
	Clamp(&me->dx,7*FIXAMT);
	Clamp(&me->dy,7*FIXAMT);

	if(me->dx<-FIXAMT)
	{
		if(me->dy<-FIXAMT)
			me->facing=5;
		else if(me->dy>FIXAMT)
			me->facing=3;
		else
			me->facing=4;
	}
	else if(me->dx>FIXAMT)
	{
		if(me->dy<-FIXAMT)
			me->facing=7;
		else if(me->dy>FIXAMT)
			me->facing=1;
		else
			me->facing=0;
	}
	else
	{
		if(me->dy<-FIXAMT)
			me->facing=6;
		else
			me->facing=2;
	}

	if(me->seq!=ANIM_MOVE)
		DoMove(me,ANIM_MOVE,180,0,me->dx,me->dy);
}

void AI_HumanLarry(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i;
	//Guy *g;

	if(me->mind==30*8+2)
	{
		//BeginChatting(235);
		SelectDestination(me,goodguy,map,1);
	}

	//Burn((me->mind2*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT,(me->mind3*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT,0);

	// if(RangeToTarget(me,goodguy)<400*FIXAMT)
	// 	SetBossHP(me->hp,MonsterHP(me->aiType));

	if(me->ouch==4)
	{
		if(me->mind>15)
			me->mind-=15;
		else
			me->mind=0;
	}
	if(me->mind)
		me->mind--;

	// if(me->mind<30)
	// {
	// 	Burn(me->x,me->y,Random(50*FIXAMT));
	// }

	BasicAI(me,SND_LARRYOUCH,SND_VILLDIE1,map,world,goodguy);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4)
		{
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=10)
		{
			Dampen(&me->dx,FIXAMT);
			Dampen(&me->dy,FIXAMT);
			if(me->AttackCheck(40,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,1,map,world);
			}
			if(me->frm==10)
			{
				me->dx=0;
				me->dy=0;
			}
		}
		if(me->seq==ANIM_A1 && (me->frm==6 || me->frm==7) && me->frmTimer<128)
		{
			x=me->x+Cosine((me->facing+7)*32)*20;
			y=me->y+Sine((me->facing+7)*32)*20;
			for(i=0;i<3;i++)
			{
				FireBullet(x,y,(me->facing*32+(me->frm-6)*8+256-32+i*(32/6))&255,BLT_CLAW,me->friendly);
			}
			x=me->x+Cosine((me->facing+1)*32)*20;
			y=me->y+Sine((me->facing+1)*32)*20;
			for(i=0;i<3;i++)
			{
				FireBullet(x,y,(me->facing*32-(me->frm-6)*8+256-16+i*(32/6))&255,BLT_CLAW,me->friendly);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && me->frm>=5 && me->frm<=15)
		{
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(16,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,1,map,world);
			}
		}
		if(me->seq==ANIM_DIE)
		{
			AddGuy(me->x,me->y,me->z,MONS_VILLAGER3,1);
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			me->type=MONS_NONE;
			// BadgeCheck(BE_KILL,me->aiType,map);
			// PlayerSetVar(VAR_QUESTDONE+QUEST_WOLF,1);
			// if(g)
			// 	g->tag=16;
			//BeginChatting(234);
		}
		return;	// can't do nothin' right now
	}

	if(me->mapx==me->mind2 && me->mapy==me->mind3)
		SelectDestination(me,goodguy,map,0);

	/*
	if(me->mind1)
	{
		me->mind1=0;
		me->dx=0;
		me->dy=0;
		SelectDestination(me,goodguy,map,1);
	}
	*/

	if(me->mind==0)
	{
		me->type=MONS_LARRY;
		me->aiType=MONS_LARRY;
		DoMove(me,ANIM_A2,128,1,0,0);
		MakeSound(SND_LARRYHOWL,me->x,me->y,SND_CUTOFF,1800);
		SelectDestination(me,goodguy,map,1);
		me->mind=30*8;
		return;
	}

	if(me->mapx>me->mind2)
		me->dx-=FIXAMT;
	else if(me->mapx<me->mind2)
		me->dx+=FIXAMT;
	if(me->mapy>me->mind3)
		me->dy-=FIXAMT;
	else if(me->mapy<me->mind3)
		me->dy+=FIXAMT;
	Dampen(&me->dx,FIXAMT/4);
	Dampen(&me->dy,FIXAMT/4);
	Clamp(&me->dx,5*FIXAMT+FIXAMT/2);
	Clamp(&me->dy,5*FIXAMT+FIXAMT/2);

	if(me->dx<-FIXAMT)
	{
		if(me->dy<-FIXAMT)
			me->facing=5;
		else if(me->dy>FIXAMT)
			me->facing=3;
		else
			me->facing=4;
	}
	else if(me->dx>FIXAMT)
	{
		if(me->dy<-FIXAMT)
			me->facing=7;
		else if(me->dy>FIXAMT)
			me->facing=1;
		else
			me->facing=0;
	}
	else
	{
		if(me->dy<-FIXAMT)
			me->facing=6;
		else
			me->facing=2;
	}

	if(me->seq!=ANIM_MOVE)
		DoMove(me,ANIM_MOVE,256,0,me->dx,me->dy);
}

void AI_Harry(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,i,a,b;
	Guy *g;

	BasicAI(me,SND_WOLFOUCH,SND_WOLFDIE,map,world,goodguy);

	if(me->mind3==0)
		me->mind3=30*4;
	if(me->mind3>1)
		me->mind3--;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4)
		{
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=6 && me->frm<=10)
		{
			Dampen(&me->dx,FIXAMT);
			Dampen(&me->dy,FIXAMT);
			if(me->AttackCheck(40,me->x>>FIXSHIFT,me->y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,3,map,world);
			}
			if(me->frm==8 && me->frmTimer<128)
			{
				x=me->x+Cosine(me->facing*32)*48;
				y=me->y+Sine(me->facing*32)*48;
				// MakeRingParticle(x,y,0,128,64);
				if(me->AttackCheck(128,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				{
					goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,3,map,world);
				}
				ShakeScreen(15);
			}
			if(me->frm==10)
			{
				me->dx=0;
				me->dy=0;
			}
		}
		if(me->seq==ANIM_A1 && me->frm==6 && me->frmTimer<128)
		{
			x=me->x+Cosine((me->facing+7+2*(7-me->frm))*32)*20;
			y=me->y+Sine((me->facing+7+2*(7-me->frm))*32)*20;
			MakeSound(SND_LARRYDARTS,me->x,me->y,SND_CUTOFF,1200);
			for(i=0;i<10;i++)
			{
				FireBullet(x,y,(me->facing*32+(me->frm-6)*4+256-24+i*(48/10))&255,BLT_CLAW,me->friendly);
			}
			me->reload=5;
		}

		if(me->seq==ANIM_A2 && me->frm>=5 && me->frm<=15 && me->reload==0)
		{
			a=Random(256);
			b=Random(60)+20;
			x=me->x+b*Cosine(a);
			y=me->y+b*Sine(a);
			g=AddGuy(x,y,FIXAMT*40,MONS_WOLFMAN3, me->friendly);
			if(g && !g->CanWalk(g->x,g->y,map,world))
				g->type=MONS_NONE;
			else
			{
				// MakeRingParticle(x,y,FIXAMT*40,20,40);
				FireBullet(x,y,0,BLT_BOOM,me->friendly);
			}
			me->reload=3;
		}
		if(me->seq==ANIM_DIE)
		{
			//x=me->x-20*FIXAMT+Random(40*FIXAMT);
			//y=me->y-20*FIXAMT+Random(40*FIXAMT);
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			if(Random(2)==0)
			{
				y=me->y-FIXAMT*10+Random(FIXAMT*11);
				x=me->x-32*FIXAMT+Random(65*FIXAMT);
				FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
			}
		}
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<64*FIXAMT && Random(4)==0)// &&
		//map->CheckLOS(me->mapx,me->mapy,5,goodguy->mapx,goodguy->mapy))
	{
		// get him!
		MakeSound(SND_LARRYPOUNCE,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,128,1,0,0);
		FaceGoodguy2(me,goodguy);
		return;
	}
	if(Random(32)==0)	// fire at any range
	{
		// shoot darts
		MakeSound(SND_LARRYPOUNCE,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A1,200,1,0,0);
		FaceGoodguy2(me,goodguy);
		return;
	}

	if(me->mind3==1)
	{
		me->mind3=0;
		MakeSound(SND_WOLFHOWL,me->x,me->y,SND_CUTOFF,1000);
		DoMove(me,ANIM_A2,128,1,0,0);
		return;
	}

	WanderAI(me,256,20,3,map,world,goodguy,4*FIXAMT);
}

void AI_Boneheadomatic(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	Guy *g;

	int x,y;

	if(me->ouch==4)
		MakeSound(SND_LIGHTSMACK,me->x,me->y,SND_CUTOFF,1200);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==22)
		{
			me->seq=ANIM_A1;
			me->frm=0;
			me->frmTimer=0;
		}
		if(me->seq==ANIM_A1 && me->frm==22 && me->frmTimer<128)
		{
			if(me->aiType==MONS_BHOMATIC)
				g=AddGuy(me->x,me->y+36*FIXAMT,0,MONS_BONEHEAD2,me->friendly);
			else if(me->aiType==MONS_RBOMATIC)
				g=AddGuy(me->x,me->y+36*FIXAMT,0,MONS_BONEHEAD3,me->friendly);
			else if(me->aiType==MONS_GGOMATIC)
				g=AddGuy(me->x,me->y+36*FIXAMT,0,MONS_BONEHEAD4,me->friendly);
			else if(me->aiType==MONS_NSOMATIC)
				g=AddGuy(me->x,me->y+36*FIXAMT,0,MONS_BONEHEAD5,me->friendly);
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				g->type=MONS_NONE;	// vanish if this spot is occupied
		}
		if(me->seq==ANIM_DIE && me->frm==1 && me->reload==0)
		{
			me->reload=4;
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		return;
	}

	if(RangeToTarget(me,goodguy)<1024*FIXAMT)
	{
		me->mind++;
		if(me->mind>30)
		{
			me->mind=0;
 			DoMove(me,ANIM_ATTACK,128,1,0,0);
		}
	}
}

void AI_LL_Vampire(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	byte damageValues[] = {2,3,3,5};
	int speedValues[] = {5*FIXAMT,7*FIXAMT,5*FIXAMT,4*FIXAMT};
	int x,y;
	Guy *g;

	BasicAI(me,SND_LL_VAMPOUCH,SND_LL_VAMPDIE,map,world,goodguy);

	if(me->mind3>0)
		me->mind3--;

	if(me->mind2>0)
		me->mind2--;

	if(me->action==ACTION_BUSY)
	{
		// shove player
		if(me->seq==ANIM_ATTACK && me->frm==4)
		{
			x=me->x+Cosine(me->facing*32)*16;
			y=me->y+Sine(me->facing*32)*16;
			if(me->AttackCheck(24,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*2,Sine(me->facing*32)*2,damageValues[me->aiType-MONS_VAMPIRE2],map,world);
			}
		}
		if(me->seq==ANIM_A1 && me->frm==4 && me->reload==0)
		{
			// summon something
			if(me->aiType==MONS_VAMPIRE4)
			{
				// summon Gangrenes, one on either side
				x=me->x-64*FIXAMT;
				y=me->y;
				g=AddGuy(x,y,0,MONS_BONEHEAD4,me->friendly);
				if(g)
				{
					if(!g->CanWalk(g->x,g->y,map,world))
						g->type=MONS_NONE;	// vanish if this spot is occupied
					//else if(!map->CheckLOS(me->mapx,me->mapy,5,g->mapx,g->mapy))
						//g->type=MONS_NONE;
					else // do resurrect move
						DoMove(g,ANIM_A3,128,1,0,0);
				}
				x=me->x+64*FIXAMT;
				y=me->y;
				g=AddGuy(x,y,0,MONS_BONEHEAD4,me->friendly);
				if(g)
				{
					if(!g->CanWalk(g->x,g->y,map,world))
						g->type=MONS_NONE;	// vanish if this spot is occupied
					// else if(!map->CheckLOS(me->mapx,me->mapy,5,g->mapx,g->mapy))
					// 	g->type=MONS_NONE;
					else // do resurrect move
						DoMove(g,ANIM_A3,128,1,0,0);
				}
			}
			if(me->aiType==MONS_VAMPIRE5)
			{
				// summon 4 ghosts
				g=AddGuy(me->x,me->y,0,MONS_GHOST2,me->friendly);
				if(g) // do end half of teleport move
				{
					DoMove(g,ANIM_A1,128,1,0,0);
					g->frm=4;
				}
				g=AddGuy(me->x,me->y,0,MONS_GHOST2,me->friendly);
				if(g) // do end half of teleport move
				{
					DoMove(g,ANIM_A1,128,1,0,0);
					g->frm=4;
				}
				g=AddGuy(me->x,me->y,0,MONS_GHOST2,me->friendly);
				if(g) // do end half of teleport move
				{
					DoMove(g,ANIM_A1,128,1,0,0);
					g->frm=4;
				}
				g=AddGuy(me->x,me->y,0,MONS_GHOST2,me->friendly);
				if(g) // do end half of teleport move
				{
					DoMove(g,ANIM_A1,128,1,0,0);
					g->frm=4;
				}
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && me->frm==2 && me->reload==0)
		{
			// shoot something
			if(me->aiType==MONS_VAMPIRE3)
			{
				// shoot bullet
				x=me->x+Cosine(me->facing*32)*16;
				y=me->y+Sine(me->facing*32)*16;
				FireBullet(x,y,me->facing*32,BLT_ENERGY,me->friendly);
			}
			if(me->aiType==MONS_VAMPIRE4)
			{
				// shoot poison gas
				x=me->x+Cosine(me->facing*32)*16;
				y=me->y+Sine(me->facing*32)*16;
				FireBullet(x,y,me->facing*32,BLT_GASBLAST,me->friendly);
			}
			if(me->aiType==MONS_VAMPIRE5)
			{
				// shoot a bat
				x=me->x+Cosine(me->facing*32)*32;
				y=me->y+Sine(me->facing*32)*32;
				g=AddGuy(x,y,0,MONS_BAT2,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				else if(g)
				{
					g->facing=me->facing;
					DoMove(g,ANIM_A1,128,1,Cosine(me->facing*32)*8,Sine(me->facing*32)*8);
				}
			}
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	// the two lower classes of vampire use shoves
	if(me->aiType<MONS_VAMPIRE4)
	{
		if(RangeToTarget(me,goodguy)<64*FIXAMT && Random(4)==0)// &&
			//map->CheckLOS(me->mapx,me->mapy,5,goodguy->mapx,goodguy->mapy))
		{
			FaceGoodguy(me,goodguy);
			if(me->aiType==MONS_VAMPIRE3)
				x=256;
			else
				x=180;
			MakeSound(SND_VAMPSHOVE,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_ATTACK,x,1,0,0);
			return;
		}
	}

	// all non-Junior vampires shoot things
	if(me->aiType>MONS_VAMPIRE2)
	{
		if(RangeToTarget(me,goodguy)<300*FIXAMT && Random(48)==0 && me->mind2==0)// &&
			//map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
		{
			// Vamp Lords are limited to one shot every second, to reduce the insanity
			if(me->aiType==MONS_VAMPIRE4)
				me->mind2=30;
			else	// 1 second for Draculites
				me->mind2=30;
			FaceGoodguy(me,goodguy);
			MakeSound(SND_VAMPSHOOT,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_A2,128,1,0,0);
			return;
		}
	}

	// everything above Bloodsucker has a summon attack
	if(me->aiType>MONS_VAMPIRE3)// && map->CheckLOS(me->mapx,me->mapy,15,goodguy->mapx,goodguy->mapy))
	{
		if(Random(320)==0 && me->mind3==0)
		{
			me->mind3=5*30;
			MakeSound(SND_VAMPSUMMON,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_A1,128,1,0,0);
			return;
		}
	}

	WanderAI(me,256,20,3,map,world,goodguy,speedValues[me->aiType-MONS_VAMPIRE2]);
}

void AI_Frankenjulie(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	// byte damageValues[] = {3};
	// int speedValues[] = {3*FIXAMT};
	int x,y;
	Guy *g;

	//SetBossHP(me->hp,MonsterHP(me->aiType));

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
		{
			if(me->mind<2)
				MakeSound(SND_FRANKOUCH,me->x,me->y,SND_CUTOFF,1200);
			else
				MakeSound(SND_FRANKOUCH2,me->x,me->y,SND_CUTOFF,1200);

			if(me->hp<monsType[me->aiType].hp/2 && me->mind==0)
			{
				me->mind=1;
				FireBullet(me->x,me->y,0,BLT_MEGABOOM,me->friendly);
				DoMove(me,ANIM_A1,128,0,0,0);
				ShakeScreen(5);
			}
			if(me->hp<monsType[me->aiType].hp/4 && me->mind==1)
			{
				me->mind=2;
				FireBullet(me->x,me->y,0,BLT_MEGABOOM,me->friendly);
				// for(x=0;x<20;x++)
				// 	FireBullet(me->x,me->y,ITM_TINYHEART,BLT_ITEM);
				DoMove(me,ANIM_A1,128,0,0,0);
				ShakeScreen(5);
				g=AddGuy(me->x-FIXAMT*150,me->y,0,MONS_HAND,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
				g=AddGuy(me->x+FIXAMT*150,me->y,0,MONS_HAND,me->friendly);
				if(g && (!g->CanWalk(g->x,g->y,map,world)))
					g->type=MONS_NONE;	// vanish if this spot is occupied
			}
		}
		else
		{
			MakeSound(SND_FRANKDIE,me->x,me->y,SND_CUTOFF,1200);
		}
	}

	if(me->mind2==0)
	{
		if(RangeToTarget(me,goodguy)<256*FIXAMT || me->ouch)
		{
			me->mind2=1;
			//BeginChatting(251);
		}
		return;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			ShakeScreen(120);
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frmTimer<128)
				MakeSound(SND_FRANKHEAD,me->x,me->y,SND_CUTOFF,400);
			if(me->frm<16 && me->frmTimer<128)
			{
				x=(me->x+Cosine(me->frm*16)*80);
				y=(me->y+Sine(me->frm*16)*60);
				FireExactBullet(x,y,FIXAMT*70,Cosine(me->frm*16)*8,Sine(me->frm*16)*8,
					-FIXAMT*2,0,60,me->frm*16,BLT_ENERGY,me->friendly);
				x=(me->x+Cosine((me->frm+8)*16)*80);
				y=(me->y+Sine((me->frm+8)*16)*60);
				FireExactBullet(x,y,FIXAMT*70,Cosine((me->frm+8)*16)*8,Sine((me->frm+8)*16)*8,
					-FIXAMT*2,0,60,(me->frm+8)*16,BLT_ENERGY,me->friendly);

			}
		}
		if(me->seq==ANIM_A3)
		{
			if(me->frm>4 && me->frm<20)
			{
				me->dx+=Cosine(me->facing*32);
				me->dy+=Sine(me->facing*32);
				Clamp(&me->dx,6*FIXAMT);
				Clamp(&me->dy,6*FIXAMT);

				x=(me->x+Cosine((me->frm-4)*16)*200)/FIXAMT;
				y=(me->y+Sine((me->frm-4)*16)*160)/FIXAMT;
				if(me->AttackCheck(50,x,y,goodguy))
					goodguy->GetShot(Cosine((me->frm-4)*16)*8,Sine((me->frm-4)*16)*8,3,map,world);
				x=(me->x+Cosine((me->frm+4)*16)*200)/FIXAMT;
				y=(me->y+Sine((me->frm+4)*16)*160)/FIXAMT;
				if(me->AttackCheck(50,x,y,goodguy))
					goodguy->GetShot(Cosine((me->frm+4)*16)*8,Sine((me->frm+4)*16)*8,3,map,world);
			}
			else if(me->frm>=20)
			{
				me->dx=(me->dx*3)/4;
				me->dy=(me->dy*3)/4;
				if(me->frm==21)
					DoMove(me,ANIM_A1,128,0,0,0);
			}
		}
		if(me->seq==ANIM_A4 && me->frm==4 && me->frmTimer<128)
		{
			x=me->x-120*FIXAMT;
			y=me->y-20*FIXAMT;
			FireBullet(x,y,128,BLT_MISSILE,me->friendly);
			FireBullet(x,y-30*FIXAMT,148,BLT_MISSILE,me->friendly);
			x=me->x+120*FIXAMT;
			y=me->y-20*FIXAMT;
			FireBullet(x,y,0,BLT_MISSILE,me->friendly);
			FireBullet(x,y-30*FIXAMT,236,BLT_MISSILE,me->friendly);
			MakeSound(SND_FRANKMISSILE,me->x,me->y,SND_CUTOFF,1200);
		}
		if(me->seq==ANIM_A4 && me->frm==10)
		{
			DoMove(me,ANIM_A2,128,0,0,0);
		}
		return;
	}
	if(RangeToTarget(me,goodguy)<96*FIXAMT)
	{
		// 'acidic'
		goodguy->GetShot(0,0,3,map,world);
	}

	FaceGoodguy2(me,goodguy);
	me->dx+=Cosine(me->facing*32)/4;
	me->dy+=Sine(me->facing*32)/4;
	Clamp(&me->dx,3*FIXAMT);
	Clamp(&me->dy,3*FIXAMT);

	if(me->mind==0)	// have head
	{
		if(me->mind1==0)
			me->mind1=30*3;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				DoMove(me,ANIM_ATTACK,128,1,me->dx,me->dy);
			}
		}
	}
	if(me->mind==1)	// lost head already
	{
		if(me->seq==ANIM_IDLE || me->seq==ANIM_MOVE || (me->seq==ANIM_A1 && me->frm>1))
		{
			DoMove(me,ANIM_A1,128,0,me->dx,me->dy);
		}
		if(me->mind1==0)
			me->mind1=30*3;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				MakeSound(SND_FRANKARMS,me->x,me->y,SND_CUTOFF,1200);
				DoMove(me,ANIM_A3,256,1,me->dx,me->dy);
				FaceGoodguy(me,goodguy);
			}
			if(me->mind1%15==0)
			{
				MakeSound(SND_GRENADE,me->x,me->y,SND_CUTOFF,1200);
				FireBullet(me->x,me->y,(byte)Random(256),BLT_GRENADE,me->friendly);
			}
		}
	}
	if(me->mind==2)	// lost body too
	{
		if(me->seq==ANIM_IDLE || me->seq==ANIM_MOVE || (me->seq==ANIM_A2 && me->frm>1))
		{
			DoMove(me,ANIM_A2,128,0,me->dx,me->dy);
		}
		if(me->mind1==0)
			me->mind1=30*2;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				DoMove(me,ANIM_A4,128,1,me->dx,me->dy);
			}
		}
	}
}

void AI_Hand(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	// byte damageValues[] = {1};
	// int speedValues[] = {5*FIXAMT};
	int x,y;

	BasicAI(me,SND_PUMPKINOUCH,SND_PUMPKINDIE,map,world,goodguy);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==7 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*40;
			y=me->y+Sine(me->facing*32)*40;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*8,Sine(me->facing*32)*8,1,map,world);
			me->reload=5;
		}
		return;	// can't do nothin' right now
	}

	if(RangeToTarget(me,goodguy)<(128*FIXAMT) && Random(20)==0)
	{
		// get him!
		MakeSound(SND_SKELKICK,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_ATTACK;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=256;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=0;
		return;
	}

	WanderAI(me,512,10,10,map,world,goodguy,5*FIXAMT);
}

void AI_MiniFrankenjulie(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	// byte damageValues[] = {3};
	// int speedValues[] = {6*FIXAMT};
	int x,y;

	//SetBossHP(me->hp,MonsterHP(me->aiType));

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
		{
			MakeSound(SND_FRANKOUCH2,me->x,me->y,SND_CUTOFF,1200);

			if(me->hp<monsType[me->aiType].hp/2 && me->mind==0)
			{
				me->mind=1;
				FireBullet(me->x,me->y,0,BLT_BOOM,me->friendly);
				DoMove(me,ANIM_A1,256,0,0,0);
			}
			if(me->hp<monsType[me->aiType].hp/4 && me->mind==1)
			{
				me->mind=2;
				FireBullet(me->x,me->y,0,BLT_BOOM,me->friendly);
				DoMove(me,ANIM_A1,256,0,0,0);
			}
		}
		else
		{
			MakeSound(SND_FRANKDIE,me->x,me->y,SND_CUTOFF,1200);
		}
	}

	if(me->mind2==0)
	{
		if(RangeToTarget(me,goodguy)<256*FIXAMT || me->ouch)
		{
			me->mind2=1;
			//BeginChatting(233);
		}
		return;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		if(me->seq==ANIM_ATTACK)
		{
			if(me->frmTimer<128)
				MakeSound(SND_FRANKHEAD,me->x,me->y,SND_CUTOFF,400);
			if(me->frm<16 && me->frmTimer<128)
			{
				x=(me->x+Cosine(me->frm*16)*40);
				y=(me->y+Sine(me->frm*16)*30);
				FireExactBullet(x,y,FIXAMT*40,Cosine(me->frm*16)*12,Sine(me->frm*16)*12,
					-FIXAMT*2,0,60,me->frm*16,BLT_ENERGY,me->friendly);
				x=(me->x+Cosine((me->frm+8)*16)*40);
				y=(me->y+Sine((me->frm+8)*16)*30);
				FireExactBullet(x,y,FIXAMT*40,Cosine((me->frm+8)*16)*12,Sine((me->frm+8)*16)*12,
					-FIXAMT*2,0,60,(me->frm+8)*16,BLT_ENERGY,me->friendly);
			}
		}
		if(me->seq==ANIM_A3)
		{
			if(me->frm>4 && me->frm<20)
			{
				me->dx+=Cosine(me->facing*32)*3;
				me->dy+=Sine(me->facing*32)*3;
				Clamp(&me->dx,9*FIXAMT);
				Clamp(&me->dy,9*FIXAMT);

				x=(me->x+Cosine((me->frm-4)*16)*100)/FIXAMT;
				y=(me->y+Sine((me->frm-4)*16)*80)/FIXAMT;
				if(me->AttackCheck(25,x,y,goodguy))
					goodguy->GetShot(Cosine((me->frm-4)*16)*8,Sine((me->frm-4)*16)*8,3,map,world);
				x=(me->x+Cosine((me->frm+4)*16)*100)/FIXAMT;
				y=(me->y+Sine((me->frm+4)*16)*80)/FIXAMT;
				if(me->AttackCheck(25,x,y,goodguy))
					goodguy->GetShot(Cosine((me->frm+4)*16)*8,Sine((me->frm+4)*16)*8,3,map,world);
			}
			else if(me->frm>=20)
			{
				me->dx=(me->dx*3)/4;
				me->dy=(me->dy*3)/4;
				if(me->frm==21)
					DoMove(me,ANIM_A1,256,0,0,0);
			}
		}
		if(me->seq==ANIM_A4 && me->frm==4 && me->frmTimer<128)
		{
			x=me->x-60*FIXAMT;
			y=me->y-10*FIXAMT;
			FireBullet(x,y,128,BLT_MISSILE,me->friendly);
			x=me->x+60*FIXAMT;
			y=me->y-10*FIXAMT;
			FireBullet(x,y,0,BLT_MISSILE,me->friendly);
			MakeSound(SND_FRANKMISSILE,me->x,me->y,SND_CUTOFF,1200);
		}
		if(me->seq==ANIM_A4 && me->frm==10)
		{
			DoMove(me,ANIM_A2,256,0,0,0);
		}
		return;
	}
	if(RangeToTarget(me,goodguy)<65*FIXAMT)
	{
		// 'acidic'
		goodguy->GetShot(0,0,3,map,world);
	}

	FaceGoodguy2(me,goodguy);
	me->dx+=Cosine(me->facing*32)/4;
	me->dy+=Sine(me->facing*32)/4;
	Clamp(&me->dx,6*FIXAMT);
	Clamp(&me->dy,6*FIXAMT);

	if(me->mind==0)	// have head
	{
		if(me->mind1==0)
			me->mind1=30*3;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				DoMove(me,ANIM_ATTACK,256,1,me->dx,me->dy);
			}
		}
	}
	if(me->mind==1)	// lost head already
	{
		if(me->seq==ANIM_IDLE || me->seq==ANIM_MOVE || (me->seq==ANIM_A1 && me->frm>1))
		{
			DoMove(me,ANIM_A1,256,0,me->dx,me->dy);
		}
		if(me->mind1==0)
			me->mind1=30*3;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				MakeSound(SND_FRANKARMS,me->x,me->y,SND_CUTOFF,1200);
				DoMove(me,ANIM_A3,160,1,me->dx,me->dy);
				FaceGoodguy(me,goodguy);
			}
			if(me->mind1%15==0)
			{
				MakeSound(SND_GRENADE,me->x,me->y,SND_CUTOFF,1200);
				FireBullet(me->x,me->y,(byte)Random(256),BLT_GRENADE,me->friendly);
			}
		}
	}
	if(me->mind==2)	// lost body too
	{
		if(me->seq==ANIM_IDLE || me->seq==ANIM_MOVE || (me->seq==ANIM_A2 && me->frm>1))
		{
			DoMove(me,ANIM_A2,256,0,me->dx,me->dy);
		}
		if(me->mind1==0)
			me->mind1=30*2;
		else
		{
			me->mind1--;
			if(me->mind1==0)
			{
				DoMove(me,ANIM_A4,256,1,me->dx,me->dy);
			}
		}
	}
}

void AI_Bonkula(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;
	dword d;

	//SetBossHP(me->hp,MonsterHP(me->aiType));

	if(me->reload)
		me->reload--;

	if(me->ouch==3)
	{
		if(me->hp>0)
			MakeSound(SND_BONKOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_BONKDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->ouch)
	{
		x=me->x-40*FIXAMT+Random(80*FIXAMT);
		y=me->y-30*FIXAMT+Random(60*FIXAMT);
		FireBullet(x,y,0,BLT_FLAMEWALL,me->friendly);
	}

	// handle introductions
	// if(player.var[240]==0)
	// {
	// 	if(me->y>goodguy->y-FIXAMT*(TILE_HEIGHT*3+TILE_HEIGHT/2))
	// 	{
	// 		player.var[240]=1;
	// 		me->dy=0;
	// 	}
	// 	else
	// 		me->dy=FIXAMT*16;
	// 	return;
	// }
	// else if(player.var[240]<10)
	// {
	// 	player.var[240]++;
	// 	if(player.var[240]==10)
	// 		BeginChatting(238);
	// 	return;
	// }

	// if(player.var[245]==1)
	// {
	// 	if(map->map[me->mapx+me->mapy*map->width].tag==10)
	// 	{
	// me->mind3=30;
	// FaceGoodguy(me,goodguy);
	// me->dx=Cosine(me->facing*32)*8;
	// me->dy=Sine(me->facing*32)*8;
	// me->dx+=-FIXAMT*2+Random(4*FIXAMT);
	// me->dy+=-FIXAMT*2+Random(4*FIXAMT);
	// monsType[me->aiType].flags&=(~MF_INVINCIBLE);
	// me->GetShot(0,0,2,map,world);
	// monsType[me->aiType].flags|=MF_INVINCIBLE;

	// if(me->reload==0)
	// {
	// 	x=me->x-300*FIXAMT+Random(600)*FIXAMT;
	// 	y=me->y+Random(900)*FIXAMT;
	// 	g=AddGuy(x,y,FIXAMT*50,MONS_VAMPIRE2+(byte)Random(3),me->friendly);
	// 	if(g && !g->CanWalk(g->x,g->y,map,world))
	// 	{
	// 		g->type=MONS_NONE;
	// 	}
	// 	me->reload=2;
	// }
	// 	}
	// }
	// get burned by the light!
	if(map->GetTile(me->mapx,me->mapy)->light>0 && me->ouch==0 && me->hp>0)
	{
		d=monsType[me->type].flags;
		monsType[me->type].flags=0;
		me->GetShot(0,0,map->GetTile(me->mapx,me->mapy)->light,map,world);
		monsType[me->type].flags=d;
		BlowSmoke(me->x,me->y,FIXAMT*10,Random(6)*FIXAMT);
		BlowSmoke(me->x,me->y,FIXAMT*10,Random(6)*FIXAMT);
		BlowSmoke(me->x,me->y,FIXAMT*10,Random(6)*FIXAMT);
		BlowSmoke(me->x,me->y,FIXAMT*10,Random(6)*FIXAMT);
		BlowSmoke(me->x,me->y,FIXAMT*10,Random(6)*FIXAMT);
	}
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==8 && me->reload==0)
		{
			MakeSound(SND_BONKHIT,me->x,me->y,SND_CUTOFF,1200);
			x=me->x+Cosine(me->facing*32)*60;
			y=me->y+Sine(me->facing*32)*60;
			FireBullet(x,y,0,BLT_SHOCKWAVE,me->friendly);

			if(me->AttackCheck(48,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(0,0,5,map,world);
			// FindVictim(x>>FIXSHIFT,y>>FIXSHIFT,6*15,Cosine(me->facing*32)*2,
			// 				   Sine(me->facing*32)*2,5,map,world,me->friendly);
			//MakeRingParticle(x,y,0,6*15,10*15);
			me->reload=5;
		}

		if(me->seq==ANIM_DIE)
		{
			me->dx=0;
			me->dy=0;
			x=me->x-40*FIXAMT+Random(80*FIXAMT);
			y=me->y-30*FIXAMT+Random(60*FIXAMT);
			FireBullet(x,y,0,BLT_FLAMEWALL,me->friendly);
			MakeSound(SND_BONKDIE,me->x,me->y-200*me->frm,SND_CUTOFF,2000);
		}
		if(me->seq==ANIM_A1)
		{
			me->facing++;
			if(me->facing>7)
				me->facing=0;
			if(Random(30)==0)
			{
				x=me->x-300*FIXAMT+Random(600)*FIXAMT;
				y=me->y-300*FIXAMT+Random(600)*FIXAMT;
				g=AddGuy(x,y,FIXAMT*50,MONS_VAMPIRE2+(byte)Random(4),me->friendly);
				if(g && !g->CanWalk(g->x,g->y,map,world))
				{
					g->type=MONS_NONE;
				}
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind3==0)
	{
		// begin spinning
		DoMove(me,ANIM_A1,256,1,0,0);
		me->mind3=60;
	}
	else
	{
		if(me->mind3==60)
		{
			FaceGoodguy(me,goodguy);
			me->dx=Cosine(me->facing*32)*8;
			me->dy=Sine(me->facing*32)*8;
			me->dx+=-FIXAMT*2+Random(4*FIXAMT);
			me->dy+=-FIXAMT*2+Random(4*FIXAMT);
		}
		me->mind3--;
		if(RangeToTarget(me,goodguy)<200*FIXAMT)
		{
			MakeSound(SND_BONKSWING,me->x,me->y,SND_CUTOFF,2000);
			DoMove(me,ANIM_ATTACK,256,1,0,0);
			me->mind3=0;
		}
	}
}

void AI_MechaBonkula(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;

	//SetBossHP(me->hp,MonsterHP(me->aiType));

	if(me->reload)
		me->reload--;

	if(me->ouch==3)
	{
		if(me->hp>0)
			MakeSound(SND_BONKOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_BONKDIE,me->x,me->y,SND_CUTOFF,1200);
	}

	// handle introductions
	// if(player.var[240]==0)
	// {
	// 	if(me->y>goodguy->y-FIXAMT*(TILE_HEIGHT*3+TILE_HEIGHT/2))
	// 	{
	// 		player.var[240]=1;
	// 		me->dy=0;
	// 	}
	// 	else
	// 		me->dy=FIXAMT*16;
	// 	return;
	// }
	// else if(player.var[240]<10)
	// {
	// 	player.var[240]++;
	// 	if(player.var[240]==10)
	// 		BeginChatting(232);
	// 	return;
	// }

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==8 && me->reload==0)
		{
			MakeSound(SND_BONKHIT,me->x,me->y,SND_CUTOFF,1200);
			x=me->x+Cosine(me->facing*32)*60;
			y=me->y+Sine(me->facing*32)*60;
			FireBullet(x,y,0,BLT_SHOCKWAVE,me->friendly);

			if(me->AttackCheck(48,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(0,0,5,map,world);
			// FindGoodVictim(x>>FIXSHIFT,y>>FIXSHIFT,6*15,Cosine(me->facing*32)*2,
			// 				   Sine(me->facing*32)*2,5,map,world),me->friendly;
			//MakeRingParticle(x,y,0,6*15,10*15);
			me->reload=5;
		}

		if(me->seq==ANIM_DIE)
		{
			me->dx=0;
			me->dy=0;
			x=me->x-40*FIXAMT+Random(80*FIXAMT);
			y=me->y-30*FIXAMT+Random(60*FIXAMT);
			//FireBullet(x,y,0,BLT_FLAMEWALL);
			ExplodeParticles(PART_YELLOW,x,y,Random(10*FIXAMT),5);
			MakeSound(SND_BONKDIE,me->x,me->y-200*me->frm,SND_CUTOFF,2000);
		}
		if(me->seq==ANIM_A1)
		{
			me->facing++;
			if(me->facing>7)
				me->facing=0;
			if(Random(30)==0)
			{
				x=me->x-300*FIXAMT+Random(600)*FIXAMT;
				y=me->y-300*FIXAMT+Random(600)*FIXAMT;
				g=AddGuy(x,y,FIXAMT*50,MONS_VAMPIRE2+(byte)Random(4),me->friendly);
				if(g && !g->CanWalk(g->x,g->y,map,world))
				{
					g->type=MONS_NONE;
				}
			}
		}
		return;	// can't do nothin' right now
	}

	if(me->mind3==0)
	{
		// begin spinning
		DoMove(me,ANIM_A1,256,1,0,0);
		me->mind3=60;
	}
	else
	{
		if(me->mind3==60)
		{
			FaceGoodguy(me,goodguy);
			me->dx=Cosine(me->facing*32)*6;
			me->dy=Sine(me->facing*32)*6;
			me->dx+=-FIXAMT*2+Random(4*FIXAMT);
			me->dy+=-FIXAMT*2+Random(4*FIXAMT);
		}
		me->mind3--;
		if(RangeToTarget(me,goodguy)<150*FIXAMT)
		{
			MakeSound(SND_BONKSWING,me->x,me->y,SND_CUTOFF,2000);
			DoMove(me,ANIM_ATTACK,256,1,0,0);
			me->mind3=0;
		}
	}
}

void AI_Evilizer(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int summonTab[]={MONS_BONEHEAD5,MONS_ZOMBIE4,MONS_MUMMY4,MONS_WOLFMAN3,MONS_SWAMPDOG3,
		MONS_GHOST4,MONS_VAMPIRE5};

	int x,y,i;
	Guy *g;

	if(me->mind==0 && CountMonsters(MONS_EVILPUMP)==0)
	{
		me->mind=1;
		me->type=MONS_EVILIZER2;
		for(i=0;i<10;i++)
		{
			//y=me->y+140*FIXAMT-Random(281*FIXAMT);
			//x=me->x-140*FIXAMT+Random(281*FIXAMT);
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		

	}

	//SetBossHP(me->hp,MonsterHP(me->aiType));

	// if(me->hp && RangeToTarget(me,goodguy)<780*FIXAMT)
	// 	SetNoSaving(1);
	// if(me->hp==0)
	// 	SetNoSaving(0);

	if(me->ouch==4)
		MakeSound(SND_METALSMACK,me->x,me->y,SND_CUTOFF,1200);

	// if(!CheckForPipes(map))
	// {
	// 	monsType[me->aiType].flags&=(~MF_INVINCIBLE);
	// 	me->mind=1;
	// 	if(opt.cheats[CH_DEATH]==0 && me->hp>2000-8*50)
	// 		me->hp=2000-8*50;
	// }

	// if(player.var[240]==8 && me->mind==0)
	// {
	// 	monsType[me->aiType].flags&=(~MF_INVINCIBLE);
	// 	me->mind=1;
	// 	if(opt.cheats[CH_DEATH]==0 && me->hp>2000-player.var[240]*50)
	// 		me->hp=2000-player.var[240]*50;
	// }
	// else if(me->mind==0)
	// {
	// 	if(opt.cheats[CH_DEATH]==0 && me->hp>2000-player.var[240]*50)
	// 		me->hp=2000-player.var[240]*50;
	// }

	// mind=whether uncovered or not
	if(me->mind==0)
		me->facing=0;
	else
		me->facing=16;

	// mind1=time to shoot an evilface off
	if(me->mind1==0)
	{
		y=me->y-FIXAMT;
		x=me->x-140*FIXAMT+Random(281*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
		me->mind1=24;
		MakeSound(SND_EVILIZER,me->x,me->y,SND_CUTOFF,300);
	}
	else
	{
		me->mind1--;
		if(!(me->mind1&7))
		{
			y=me->y-FIXAMT;
			x=me->x-140*FIXAMT+Random(281*FIXAMT);
			FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
		}
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			//y=me->y+140*FIXAMT-Random(281*FIXAMT);
			//x=me->x-140*FIXAMT+Random(281*FIXAMT);
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
	}

	// mind2=grenade timer (faster as it gets hurt more)
	if(me->mind2==0)
		me->mind2=30+me->hp/10;
	else
	{
		me->mind2--;
		if(me->mind2==0)
		{
			x=180*FIXAMT;
			y=180*FIXAMT;
			FireBullet(me->x-x,me->y,64+(byte)Random(128),BLT_GRENADE,me->friendly);
			FireBullet(me->x+x,me->y,192+(byte)Random(128),BLT_GRENADE,me->friendly);
			FireBullet(me->x,me->y-y,128+(byte)Random(128),BLT_GRENADE,me->friendly);
			FireBullet(me->x,me->y+y,(byte)Random(128),BLT_GRENADE,me->friendly);
			MakeSound(SND_GRENADE,me->x,me->y,SND_CUTOFF,1200);
		}
	}

	// mind3=missile timer (only works once exposed)
	if(me->mind==1)
	{
		if(me->mind3==0)
			me->mind3=60+me->hp/20;
		else
		{
			me->mind3--;
			if(me->mind3==0)
			{
				x=100*FIXAMT;
				y=40*FIXAMT;
				FireBullet(me->x-x,me->y-y,160,BLT_MISSILE,me->friendly);
				FireBullet(me->x+x,me->y-y,224,BLT_MISSILE,me->friendly);
				FireBullet(me->x-x,me->y+y,96,BLT_MISSILE,me->friendly);
				FireBullet(me->x+x,me->y+y,32,BLT_MISSILE,me->friendly);
				MakeSound(SND_FRANKMISSILE,me->x,me->y,SND_CUTOFF,1200);
			}
		}

		// reload=summon super duper monsters, only when exposed
		if(me->reload==0)
			me->reload=30*2;
		else
		{
			me->reload--;
			if(me->reload==0)
			{
				x=((Random(map->width-6)+3)*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
				y=((Random(map->height-20)+10)*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;
				g=AddGuy(x,y,FIXAMT*40,summonTab[Random(7)],me->friendly);
				if(g && !g->CanWalk(g->x,g->y,map,world))
					g->type=MONS_NONE;
			}

		}
	}
}

void AI_EvilPump(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	if(me->reload)
		me->reload--;

	FaceGoodguy3(me,goodguy);

	if(me->ouch==4)
		MakeSound(SND_METALSMACK,me->x,me->y,SND_CUTOFF,1200);

	if(me->seq==ANIM_IDLE && me->frm==8 && me->frmTimer<128)
	{
		for(y=0;y<4;y++)
			{
				x=Random(64);
				y=Random(64);
				//FireBullet(me->x,me->y,x,BLT_SWAMPGAS,me->friendly);
				FireBullet(me->x + x*FIXAMT,me->y + (y+1)*FIXAMT,32,BLT_SWAMPGAS,me->friendly);
			}
		//map->GasSpray(me->mapx,me->mapy);
		me->reload=3;
	}
	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && (me->frm==3 || me->frm==5 || me->frm==7) && me->reload==0)
		{
			MakeSound(SND_MUMMYSHOOT,me->x,me->y,SND_CUTOFF,1200);
			x=me->x+Cosine(me->facing*32)*48;
			y=me->y+Sine(me->facing*32)*48;
			FireBullet(x,y,(byte)(me->facing*32+240+Random(33)),BLT_BIGSHOT,me->friendly);
			me->reload=3;
			if(me->frm==7)
				me->reload=30*3;
		}
		if(me->seq==ANIM_DIE)
		{
			if(me->frm==0)
				me->reload=0;
			//x=me->x-20*FIXAMT+Random(40*FIXAMT);
			//y=me->y-20*FIXAMT+Random(40*FIXAMT);
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			if(me->frm==4 && me->reload==0)
			{
				me->reload=10;
			for(y=0;y<4;y++)
				{
					x=Random(256);
					FireBullet(me->x,me->y,x,BLT_SWAMPGAS,me->friendly);
				}
				//map->GasSpray2(me->mapx,me->mapy);
			}
		}
		return;
	}

	if(me->reload==0)
	{
		DoMove(me,ANIM_ATTACK,128,1,0,0);
	}
}

void AI_WindElder(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;

	//SetBossHP(me->hp,MonsterHP(me->aiType));

	BasicAI(me,SND_ELDEROUCH,SND_ELDERDIE,map,world,goodguy);

	// if(player.var[240]==0)
	// {
	// 	if(RangeToTarget(me,goodguy)<256*FIXAMT || me->ouch)
	// 	{
	// 		player.var[240]=1;
	// 		BeginChatting(243);
	// 	}
	// 	return;
	// }

	if(me->mind3)
		me->mind3--;
	else
		me->mind3=30*6;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm<6)
		{
			me->dx=Cosine(me->facing*32)*(6+me->frm*2);
			me->dy=Sine(me->facing*32)*(6+me->frm*2);
		}
		else if(me->seq==ANIM_ATTACK)
		{
			me->dx/=2;
			me->dy/=2;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=3 && me->frm<=6)
		{
			x=me->x+Cosine(me->facing*32)*30;
			y=me->y+Sine(me->facing*32)*30;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*16,Sine(me->facing*32)*16,3,map,world);
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frmTimer<128)
		{
			x=me->x-400*FIXAMT+Random(800*FIXAMT);
			y=me->y-300*FIXAMT+Random(600*FIXAMT);
			g=AddGuy(x,y,0,MONS_GHOST2,me->friendly);
			if(g && !g->CanWalk(g->x,g->y,map,world))
				g->type=MONS_NONE;
		}
		if(me->seq==ANIM_A2 && me->frm==2 && me->reload==0)
		{
			FireBullet(me->x,me->y,(me->facing*32-96+256)&255,BLT_WIND,me->friendly);
			FireBullet(me->x,me->y,(me->facing*32-72+256)&255,BLT_WIND,me->friendly);
			FireBullet(me->x,me->y,(me->facing*32+96+256)&255,BLT_WIND,me->friendly);
			FireBullet(me->x,me->y,(me->facing*32+72+256)&255,BLT_WIND,me->friendly);
			me->reload=4;
		}
		if(me->seq==ANIM_DIE)
		{
			//x=me->x-40*FIXAMT+Random(80*FIXAMT);
			//y=me->y-30*FIXAMT+Random(60*FIXAMT);
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			MakeSound(SND_ELDERDIE,me->x,me->y-200*me->frm,SND_CUTOFF,2000);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind3==0)
	{
		// summon hordes of ghosts
		MakeSound(SND_ELDERSUMMON,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A1,128,1,0,0);
		return;
	}

	if(RangeToTarget(me,goodguy)<(300*FIXAMT) && Random(20)==0)
	{
		// charge at him
		MakeSound(SND_ELDERCHARGE,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,128,1,0,0);
		return;
	}
	if(Random(50)==0)
	{
		// fire wind missiles
		MakeSound(SND_ELDERSHOOT,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A2,128,1,0,0);
		return;
	}

	WanderAI(me,1024,2,1,map,world,goodguy,5*FIXAMT);
}

void AI_EarthElder(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	//SetBossHP(me->hp,MonsterHP(me->aiType));

	BasicAI(me,SND_ELDEROUCH,SND_ELDERDIE,map,world,goodguy);

	// if(player.var[240]==0)
	// {
	// 	if(RangeToTarget(me,goodguy)<256*FIXAMT || me->ouch)
	// 	{
	// 		player.var[240]=1;
	// 		BeginChatting(242);
	// 	}
	// 	return;
	// }

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0)
		{
			FireBullet(me->x,me->y,(me->facing*32),BLT_EARTHSPIKE,me->friendly);
			FireBullet(me->x,me->y,(me->facing*32-32+256)&255,BLT_EARTHSPIKE,me->friendly);
			FireBullet(me->x,me->y,(me->facing*32+32)&255,BLT_EARTHSPIKE,me->friendly);
			me->reload=5;
		}

		if(me->seq==ANIM_A1 && me->frm==3 && me->reload==0)
		{
			for(x=0;x<6;x++)
				FireBullet(me->x,me->y,(byte)Random(256),BLT_GRENADE,me->friendly);
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && me->frm<9)
		{
			x=me->x+Cosine(me->facing*32)*me->frm*48;
			y=me->y+Sine(me->facing*32)*me->frm*32;
			if(me->frm==8 && me->reload==0)
			{
				me->reload=3;
				FireBullet(x,y,0,BLT_MEGABOOM,me->friendly);
			}
			else if(me->frmTimer<128)
				FireBullet(x,y,0,BLT_BOOM,me->friendly);
		}
		if(me->seq==ANIM_DIE)
		{
			MakeSound(SND_ELDERDIE,me->x,me->y-200*me->frm,SND_CUTOFF,2000);
			// doesn't explode
		}
		return;	// can't do nothin' right now
	}

	// if(me->y<(7*TILE_HEIGHT+TILE_HEIGHT*3/4)*FIXAMT)
	// {
	// 	BeginChatting(241);
	// 	DoMove(me,ANIM_DIE,128,1,0,0);
	// 	KilledBoss(BOSS_ELDER2);
	// 	me->hp=0;
	// }

	if(me->mind3)
		me->mind3-=2;
	else
		me->mind3=30*8;

	if(me->mind3==30*5 || me->mind3==30*3)
	{
		// launch grenades
		MakeSound(SND_ELDERSUMMON,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A1,128,1,0,0);
		return;
	}

	if(me->mind3==30*8 || me->mind3==30*6 || me->mind3==30*2)
	{
		// shoot earth spikes
		MakeSound(SND_ELDERCHARGE,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,128,1,0,0);
		return;
	}

	FaceGoodguy(me,goodguy);

	me->dx=0;
	me->dy=0;
}

void AI_WaterElder(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;
	Guy *g;

	//SetBossHP(me->hp,MonsterHP(me->aiType));

	BasicAI(me,SND_ELDEROUCH,SND_ELDERDIE,map,world,goodguy);

	// if(player.var[240]==0)
	// {
	// 	if(RangeToTarget(me,goodguy)<256*FIXAMT || me->ouch)
	// 	{
	// 		player.var[240]=1;
	// 		BeginChatting(240);
	// 	}
	// 	return;
	// }

	if(me->mind3)
		me->mind3--;
	else
		me->mind3=30*6;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm>=3 && me->frm<=6)
		{
			FireBullet(me->x,me->y,me->facing*32+256-8+(byte)Random(16),BLT_ICESHARD,me->friendly);
			me->reload=5;
		}
		if(me->seq==ANIM_A2 && me->frm==2 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*30;
			y=me->y+Sine(me->facing*32)*30;
			g=AddGuy(x,y,FIXAMT*10,MONS_BUBBLE,me->friendly);
			if(g && g->CanWalk(g->x,g->y,map,world))
			{
				g->dx=Cosine(me->facing*32)*4;
				g->dy=Sine(me->facing*32)*4;
			}
			else if(g)
				g->type=MONS_NONE;
			me->reload=4;
		}
		if(me->seq==ANIM_DIE)
		{
			//x=me->x-40*FIXAMT+Random(80*FIXAMT);
			//y=me->y-30*FIXAMT+Random(60*FIXAMT);
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		return;	// can't do nothin' right now
	}

	if(Random(40)==0)
	{
		// fire ice shards
		MakeSound(SND_ELDERCHARGE,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,128,1,0,0);
		return;
	}
	if(Random(30)==0)
	{
		// fire bubbles
		MakeSound(SND_ELDERSHOOT,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A2,128,1,0,0);
		return;
	}

	FaceGoodguy2(me,goodguy);
	me->dx+=Cosine(me->facing*32)*3/2;
	me->dy+=Sine(me->facing*32)*3/2;
	Clamp(&me->dx,FIXAMT*6);
	Clamp(&me->dy,FIXAMT*6);
}

void AI_Bubble(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->mind1&1)
		me->dx=-me->dx;
	if(me->mind1&2)
		me->dy=-me->dy;

	me->mind3++;
	if(me->mind3==30)
	{
		me->mind3=0;
		me->mind++;
	}

	if(me->seq==ANIM_DIE)
	{
		ExplodeParticles(PART_WATER,me->x,me->y,me->z,4);
		return;
	}
	if(me->mind==15)	// last 15 seconds
	{
		MakeSound(SND_BUBBLEPOP,me->x,me->y,SND_CUTOFF,1200);
		me->hp=0;
		DoMove(me,ANIM_DIE,128,1,0,0);
		me->dx=0;
		me->dy=0;
	}

	me->mind1=0;

	if(RangeToTarget(me,goodguy)<40*FIXAMT)
	{
		goodguy->dx+=me->dx*2;
		goodguy->dy+=me->dy*2;
		FaceGoodguy(me,goodguy);

		me->dx=-Cosine(me->facing*32)*4;
		me->dy=-Sine(me->facing*32)*4;
	}
	if(me->z==0)
		me->dz=FIXAMT*8;
}

void AI_FireElder(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	//SetBossHP(me->hp,MonsterHP(me->aiType));

	BasicAI(me,SND_ELDEROUCH,SND_ELDERDIE,map,world,goodguy);

	map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,(me->y/TILE_HEIGHT)>>FIXSHIFT,16,8);

	// if(player.var[240]==0)
	// {
	// 	if(RangeToTarget(me,goodguy)<256*FIXAMT || me->ouch)
	// 	{
	// 		player.var[240]=1;
	// 		BeginChatting(239);
	// 	}
	// 	return;
	// }

	if(me->ouch && me->action==ACTION_BUSY && me->seq!=ANIM_DIE && me->seq!=ANIM_ATTACK && Random(10)==0)
	{
		// run away from the water
		DoMove(me,ANIM_ATTACK,128,1,0,0);
		me->facing=(byte)Random(8);
		return;
	}
	if(me->mind3)
		me->mind3--;
	else
		me->mind3=30*8;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm<6)
		{
			me->dx=Cosine(me->facing*32)*(6+me->frm*2);
			me->dy=Sine(me->facing*32)*(6+me->frm*2);
		}
		else if(me->seq==ANIM_ATTACK)
		{
			me->dx/=2;
			me->dy/=2;
		}
		if(me->seq==ANIM_ATTACK && me->frm>=3 && me->frm<=6)
		{
			x=me->x+Cosine(me->facing*32)*30;
			y=me->y+Sine(me->facing*32)*30;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
				goodguy->GetShot(Cosine(me->facing*32)*16,Sine(me->facing*32)*16,5,map,world);
			me->reload=5;
		}
		if(me->seq==ANIM_A1 && me->frm==1)
			me->mind2=me->facing*32;
		if(me->seq==ANIM_A1 && me->frm>0 && me->frm<22)
		{
			me->mind2+=8;
			me->facing=me->mind2/32;
			x=me->x+20*Cosine(me->mind2);
			y=me->y+20*Sine(me->mind2);
			FireBullet(x,y,me->facing,BLT_FLAME,me->friendly);
		}

		if(me->seq==ANIM_A2 && me->frm>4 && me->frm<9 && me->frmTimer<128)
		{
			x=me->x+Cosine(me->facing*32)*(me->frm-4)*48;
			y=me->y+Sine(me->facing*32)*(me->frm-4)*32;
			if(me->frm==8 && me->reload==0)
			{
				me->reload=3;
				FireBullet(x,y,0,BLT_MEGABOOM,me->friendly);
			}
			else if(me->frmTimer<128)
				FireBullet(x,y,0,BLT_BOOM,me->friendly);
		}

		if(me->seq==ANIM_DIE)
		{
			//x=me->x-40*FIXAMT+Random(80*FIXAMT);
			//y=me->y-30*FIXAMT+Random(60*FIXAMT);
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			MakeSound(SND_ELDERDIE,me->x,me->y-200*me->frm,SND_CUTOFF,2000);
		}
		return;	// can't do nothin' right now
	}

	x=me->x-40*FIXAMT+Random(80*FIXAMT);
	y=me->y-30*FIXAMT+Random(60*FIXAMT);
	FireBullet(x,y,0,BLT_FLAMEWALL,me->friendly);

	if(me->mind3==0 || me->mind3==30*4)
	{
		// unleash flame circle
		MakeSound(SND_ELDERSUMMON,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A1,128,1,0,0);
		return;
	}

	/*
	if(RangeToTarget(me,goodguy)<(300*FIXAMT) && Random(20)==0)
	{
		// charge at him
		MakeSound(SND_ELDERCHARGE,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,128,1,0,0);
		return;
	}
	*/

	if(me->mind3==30*2 || me->mind3==30*6)
	{
		// fire explosion stream
		MakeSound(SND_ELDERSHOOT,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A2,128,1,0,0);
		return;
	}

	WanderAI(me,1024,2,1,map,world,goodguy,4*FIXAMT);
}

void AI_SummonElder(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y,a,b;
	Guy *g;

	// if(RangeToTarget(me,goodguy)<400*FIXAMT)
	// 	SetBossHP(me->hp,MonsterHP(me->aiType));

	BasicAI(me,SND_ELDEROUCH,SND_ELDERDIE,map,world,goodguy);

	if(me->seq==ANIM_DIE && Random(5)==0)
	{
		y=me->y-FIXAMT*10+Random(FIXAMT*11);
		x=me->x-32*FIXAMT+Random(65*FIXAMT);
		FireBullet(x,y,0,BLT_EVILFACE,me->friendly);
	}

	map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,(me->y/TILE_HEIGHT)>>FIXSHIFT,8,8);

	if(me->mind3)
		me->mind3--;
	else
		me->mind3=30*4;

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_A1 && me->frm>0 && me->frm<22 && me->reload==0)
		{
			a=Random(256);
			b=Random(60)+20;
			x=me->x+b*Cosine(a);
			y=me->y+b*Sine(a);

			g=AddGuy(x,y,FIXAMT*40,MONS_VAMPIRE2+me->mind2,me->friendly);
			if(g && !g->CanWalk(g->x,g->y,map,world))
				g->type=MONS_NONE;
			else
			{
				//MakeRingParticle(x,y,FIXAMT*40,20,40);
				FireBullet(x,y,0,BLT_BOOM,me->friendly);
			}
			me->reload=(me->mind2+1)*4;
		}

		if(me->seq==ANIM_DIE)
		{
			//x=me->x-40*FIXAMT+Random(80*FIXAMT);
			//y=me->y-30*FIXAMT+Random(60*FIXAMT);
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
			MakeSound(SND_ELDERDIE,me->x,me->y-200*me->frm,SND_CUTOFF,2000);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind3==0 || me->mind3==30*4)
	{
		// summon minions
		MakeSound(SND_ELDERSUMMON,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_A1,128,1,0,0);
		me->mind2=(byte)Random(4);
		return;
	}

	WanderAI(me,1024,2,1,map,world,goodguy,2*FIXAMT);
}

void AI_Villager(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	if(me->reload)
		me->reload--;

	if(me->ouch==4 && me->hp==0)
	{
		if(me->aiType>MONS_VILLAGER3)
		{
			MakeSound(SND_VILLDIE1,me->x,me->y,SND_CUTOFF,1200);
		}
		else
		{
			MakeSound(SND_VILLDIE2,me->x,me->y,SND_CUTOFF,1200);
		}
	}
		

	if(me->action==ACTION_BUSY)
	{
		return;	// can't do nothin' right now
	}

	if(me->reload==0 && Random(64)==0 && me->mind3==0)	// scared girl doesn't yawn
	{
		if(me->aiType<MONS_VILLAGER4)
			MakeSound(SND_VILLAGERSCRATCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_VILLAGERYAWN,me->x,me->y,SND_CUTOFF,1200);
		me->seq=ANIM_A1;
		me->frm=0;
		me->frmTimer=0;
		me->frmAdvance=128;
		me->action=ACTION_BUSY;
		me->dx=0;
		me->dy=0;
		me->reload=(byte)(30*4+Random(136));
		return;
	}

	if(!(me->mind2--) || (me->mind1>0 && me->mind3==0))	// time to get a new direction
	{
		me->facing=(byte)Random(8);
		me->mind2=(byte)Random(40)+20;
		me->mind=(byte)Random(4);
		if(me->mind==1)
			me->mind=2;
		if(me->mind3 && me->mind>0)
			me->mind=(byte)Random(4)+3;

	}

	if(me->mind3==0 && RangeToTarget(me,goodguy)<FIXAMT*60)
	{
		me->mind=0;
		FaceGoodguy(me,goodguy);
	}

	me->dx=(Cosine(me->facing*32)*me->mind*FIXAMT)/FIXAMT;
	me->dy=(Sine(me->facing*32)*me->mind*FIXAMT)/FIXAMT;
	if(me->mind==0)
	{
		if(me->seq!=ANIM_IDLE)
		{
			me->seq=ANIM_IDLE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=128;
		}
	}
	else
	{
		if(me->seq!=ANIM_MOVE)
		{
			me->seq=ANIM_MOVE;
			me->frm=0;
			me->frmTimer=0;
			me->frmAdvance=me->mind*32;
		}
	}
	me->mind1=0;

	if(me->mind3)
	{
		// follow obnoxiously
		FaceGoodguy(me,goodguy);
		//player.var[250]=1;	// a variable to keep track of her being obnoxious
	}
	// villagers have lanterns in dark places, until they give it to the player
	// only in Gloomy Cavern though
	if((map->flags&MAP_TORCHLIT))// && (player.var[VAR_QUESTDONE+QUEST_RESCUE]==0) &&
		//player.levelNum==24)
		map->TempTorch(me->mapx,me->mapy, 4);
}

void AI_Polterguy(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i;
	int x,y;
	Guy *g;

	//SetBossHP(me->hp,MonsterHP(me->aiType));

	BasicAI(me,SND_SPOOKOUCH,SND_SPOOKDIE,map,world,goodguy);

	if(me->mind==0)
	{
		me->mind=1;
		//BeginChatting(249);
		for(i=0;i<15;i++)
			LaunchJunk(map,me);
		me->reload=60;
		me->mind3=120+15;
		me->facing=(byte)Random(7);
		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
	}

	if(me->mind1)
	{
		if(me->mind1&1)
			me->dx=-me->dx;
		if(me->mind1&2)
			me->dy=-me->dy;

		if(Random(20)==0)
		{
			me->facing=(byte)Random(7);
			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
		}
		me->mind1=0;
	}

	if(RangeToTarget(me,goodguy)<128*FIXAMT)
	{
		goodguy->poison=32*3;
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0)
		{
			//LaunchJunk(map,me);
			LaunchJunk(map,me);
			LaunchJunk(map,me);
			me->reload=30*2;

			x=me->x-300*FIXAMT+Random(600)*FIXAMT;
			y=me->y-200*FIXAMT+Random(500)*FIXAMT;
			g=AddGuy(x,y,FIXAMT*50,MONS_GHOST2,me->friendly);
			if(g && !g->CanWalk(g->x,g->y,map,world))
			{
				g->type=MONS_NONE;
			}
			else if(g)
			{
				DoMove(g,ANIM_A1,128,1,0,0);
				g->frm=4;
			}
		}
		if(me->seq==ANIM_A1 && me->frm==15 && me->frmTimer<128)
		{
			Sneeze();
		}
		return;	// can't do nothin' right now
	}

	if(me->reload==0)
	{
		MakeSound(SND_POLTERGET,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,128,1,me->dx,me->dy);
	}
	if(me->mind3)
	{
		me->mind3--;
		if(me->mind3==0)
		{
			MakeSound(SND_POLTERSNEEZE,me->x,me->y,SND_CUTOFF,1200);
			DoMove(me,ANIM_A1,128,1,me->dx,me->dy);
			me->mind3=120;
		}
	}
}

void AI_Junk(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	BasicAI(me,SND_LIGHTSMACK,SND_BULLETHIT,map,world,goodguy);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE)
		{
			ExplodeParticles(PART_DIRT,me->x,me->y,me->z,16);
		}
		return;
	}

	if(me->parent==NULL)
	{
		me->parent=GetGuyOfType(MONS_BIGGHOST);
	}

	if(me->parent==NULL || me->parent->hp==0)
	{
		me->hp=0;
		DoMove(me,ANIM_DIE,128,1,0,0);
		return;
	}
	if(RangeToTarget(me,goodguy)<20*FIXAMT)
	{
		goodguy->GetShot(me->dx,me->dy,2,map,world);
	}

	me->frmAdvance=180;
	FaceGoodguy(me,me->parent);
	me->dx+=Cosine(me->facing*32)/2;
	me->dy+=Sine(me->facing*32)/2;
	// circling Polterguy
	if(me->mind==0)
	{
		if(me->x<me->parent->x)
		{
			// on left side
			if(me->y<me->parent->y)
			{
				// upper left
				me->dy+=FIXAMT;
			}
			else
				// lower left
				me->dx+=FIXAMT;
		}
		else
		{
			// on right side
			if(me->y<me->parent->y)
			{
				// upper left
				me->dx-=FIXAMT;
			}
			else
				// lower left
				me->dy-=FIXAMT;
		}
		Dampen(&me->dx,FIXAMT/4);
		Dampen(&me->dy,FIXAMT/4);
		Clamp(&me->dx,FIXAMT*11);
		Clamp(&me->dy,FIXAMT*11);
	}
	else
	{
		// flying outward
		me->mind1--;
		if(me->mind1==0)
			me->mind=0;
	}
}

void AI_Ghastly(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int i;
	int x,y;
	Guy *g;

	//SetBossHP(me->hp,MonsterHP(me->aiType));

	BasicAI(me,SND_SPOOKOUCH,SND_SPOOKDIE,map,world,goodguy);

	if(me->mind==0 && RangeToTarget(me,goodguy)<256*FIXAMT)
	{
		me->mind=1;
		me->reload=60;
		me->mind3=120+15;
		me->facing=(byte)Random(256);
		me->dx=Cosine(me->facing)*2;
		me->dy=Sine(me->facing)*2;
	}

	if(me->mind1)
	{
		if(me->mind1&1)
			me->dx=-me->dx;
		if(me->mind1&2)
			me->dy=-me->dy;

		if(Random(20)==0)
		{
			me->facing=(byte)Random(7);
			me->dx=Cosine(me->facing*32)*2;
			me->dy=Sine(me->facing*32)*2;
		}
		me->mind1=0;
	}

	if(Random(4)==0)
		FireBullet(me->x,me->y,(byte)Random(256),BLT_GASBLAST,me->friendly);

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==4 && me->reload==0)
		{
			me->reload=30*4;

			for(i=0;i<3;i++)
			{
				x=me->x-30*FIXAMT+Random(61)*FIXAMT;
				y=me->y-20*FIXAMT+Random(51)*FIXAMT;
				g=AddGuy(x,y,FIXAMT*30,MONS_BAT5,me->friendly);
				if(g && !g->CanWalk(g->x,g->y,map,world))
				{
					g->type=MONS_NONE;
				}
			}
		}
		if(me->seq==ANIM_A1 && me->frm==15 && me->frmTimer<128)
		{
			for(i=0;i<16;i++)
				FireBullet(me->x,me->y,i*16,BLT_GASBLAST,me->friendly);
		}
		return;	// can't do nothin' right now
	}

	if(me->reload==0)
	{
		MakeSound(SND_POLTERGET,me->x,me->y,SND_CUTOFF,1200);
		DoMove(me,ANIM_ATTACK,128,1,me->dx,me->dy);
	}
	if(Random(50)==0)
	{
		me->facing=(byte)Random(7);
		me->dx=Cosine(me->facing*32)*2;
		me->dy=Sine(me->facing*32)*2;
	}
}

void AI_LaserTurret(Guy *me,Map *map,world_t *world,Guy *goodguy)
{
	int x,y;

	me->ignited=0;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_ROBOOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_ROBODIE,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->frmTimer<63)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		return;	// can't do nothin' right now
	}

	me->facing=((me->type-MONS_TURRETR)*2);
	FireBullet(me->x,me->y,me->facing,BLT_LASER2,me->friendly);
}