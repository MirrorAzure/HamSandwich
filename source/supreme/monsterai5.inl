void BasicAI(Guy *me,byte ouchSound,byte dieSound,Map *map,world_t *world,Guy *goodguy)
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
	int x,y;

	if(me->reload)
		me->reload--;

	if(me->ouch==4)
	{
		if(me->hp>0)
			MakeSound(SND_BATOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_BATOUCH,me->x,me->y,SND_CUTOFF,1200);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_DIE && me->frm==4 && me->frmTimer<60)
		{
			x=me->x>>FIXSHIFT;
			y=me->y>>FIXSHIFT;
			BlowUpGuy(x+me->rectx,y+me->recty,x+me->rectx2,y+me->recty2,me->z,1);
		}
		return;	// can't do nothin' right now
	}

	if(me->mind==0)		// when mind=0, singlemindedly roll towards Bouapha
	{
		if(goodguy)
		{
			FaceGoodguy(me,goodguy);
			if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(32)==0 && me->reload==0)
			{
				// shoot at him
				MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,1200);
				FireBullet(me->x,me->y,me->facing,BLT_FLAME2,me->friendly);
				me->seq=ANIM_ATTACK;
				me->reload=10;
				return;
			}
			me->dx=Cosine(me->facing*32)*5;
			me->dy=Sine(me->facing*32)*5;
			if(me->seq!=ANIM_MOVE)
			{
				me->seq=ANIM_MOVE;
				me->frm=0;
				me->frmTimer=0;
				me->frmAdvance=128;
			}
			if(Random(48)==0)
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
			if(RangeToTarget(me,goodguy)<(512*FIXAMT) && Random(256)==0 && me->reload==0)
			{
				// shoot at him
				MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,1200);
				FireBullet(me->x,me->y,me->facing,BLT_FLAME2,me->friendly);
				me->seq=ANIM_ATTACK;
				me->reload=10;
				return;
			}
		}
		if(!(me->mind1--))	// time to get a new direction
		{
			if((goodguy) && Random(3)==0)
				me->mind=0;	// get back on track
			else
				me->facing=(byte)Random(8);
			me->mind1=Random(40)+1;
		}

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

	if(me->type==MONS_TRAPAZOID2)
	{
		if(me->hp==0)
			return;	// don't sweat it if you dead
			

		
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
		if(me->ouch==4)
		{
			if(me->hp>0)
				MakeSound(SND_SERPENTOUCH,me->x,me->y,SND_CUTOFF,1200);
			else
				MakeSound(SND_SERPENTDIE,me->x,me->y,SND_CUTOFF,1200);
		}
		

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
			MakeSound(SND_PYGMYOUCH,me->x,me->y,SND_CUTOFF,1200);
		else
			MakeSound(SND_PYGMYDIE,me->x,me->y,SND_CUTOFF,1200);
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
			MakeSound(SND_PYGMYYAWN,me->x,me->y,SND_CUTOFF,1200);
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
			MakeSound(SND_PYGMYYAWN,me->x,me->y,SND_CUTOFF,1200);
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
/*
	if(!me->ignited)
	{
		if(Random(100)==0)
			AddBaby(me->x+((-8+Random(17))<<FIXSHIFT),me->y+((-8+Random(17))<<FIXSHIFT),0,MONS_SPIDER,me);
		if(Random(300)==0)
			AddBaby(me->x+((-8+Random(17))<<FIXSHIFT),me->y+((-8+Random(17))<<FIXSHIFT),0,MONS_BIGSPDR,me);
	}
*/
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

	if(me->type==MONS_POLTERGUY2)
	{
		// heal in ghost form
		if(me->hp<me->maxHP && me->mind2==0)
		{
			i=me->hp;
			me->hp+=10;
			if(me->hp>=me->maxHP){
				me->type=MONS_POLTERGUY;
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
			me->type=MONS_POLTERGUY2;
			//me->ignited=0;
			MakeSound(SND_SPOOKSHIFT,me->x,me->y,SND_CUTOFF,2000);
			map->BrightTorch(me->mapx,me->mapy,16,6);
			if(me->mind3>50)
				me->mind3-=50;
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

	/*if(me->ignited)
		me->ignited=0;*/

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

	minions=ArrangeBats(AngleFrom(me->x,me->y,goodguy->x,goodguy->y));

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

		if(me->seq==ANIM_DIE)
		{
			if(me->reload>4)
				me->reload=4;

			if(me->reload==0)
			{
				g=GetGuyOfType(MONS_BATGUARD);
				if(g!=NULL)
				{
					g->hp=1;
					g->GetShot(0,0,100,map,world);
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

	if(me->type==MONS_SWAMPDOG4 && me->seq==ANIM_DIE && Random(10)==0)
	{
		y=me->y-FIXAMT*20+Random(FIXAMT*41);
		x=me->x-32*FIXAMT+Random(65*FIXAMT);
		//FireBullet(x,y,0,BLT_EVILFACE);
	}

	if(me->action==ACTION_BUSY)
	{
		if(me->seq==ANIM_ATTACK && me->frm==3 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*32;
			y=me->y+Sine(me->facing*32)*32;
			if(me->AttackCheck(32,x>>FIXSHIFT,y>>FIXSHIFT,goodguy))
			{
				goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,damageValues[me->type-MONS_SWAMPDOG],map,world);
			}
			me->reload=5;
		}
		if(me->seq==ANIM_A3 && me->frm==6 && me->reload==0)
		{
			x=me->x+Cosine(me->facing*32)*8;
			y=me->y+Sine(me->facing*32)*8;

			if(me->type==MONS_SWAMPDOG3 || me->type==MONS_SWAMPDOG4)
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
				if(me->type==MONS_SWAMPDOG4)
					FireBullet(x,y,z,BLT_BIGSHELL, me->friendly);
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
				me->dx=Cosine(me->facing*32)*speedValues[me->type-MONS_SWAMPDOG]/FIXAMT;
				me->dy=Sine(me->facing*32)*speedValues[me->type-MONS_SWAMPDOG]/FIXAMT;
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
			if(me->type!=MONS_SWAMPDOG3)
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
		if(me->type==MONS_SWAMPDOG3)
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
			goodguy->GetShot(Cosine(me->facing*32)*4,Sine(me->facing*32)*4,damageValues[me->type-MONS_SWAMPDOG],map,world);
		}
	}
}