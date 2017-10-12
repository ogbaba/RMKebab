#include <SPI.h>
#include <Gamebuino.h>
#include <EEPROM.h>

#define POS_X_BON 2
#define POS_Y_BON_0 2
#define POS_Y_BON_1 18
#define POS_Y_BON_2 34
#define POS_X_AMEL 70
#define POS_Y_AMEL 2
#define POS_X_ART 48
#define POS_Y_ART 16
#define LARG_BOITE 12
#define LARG_CURS LARG_BOITE + 2

#define PRIX_N0_BON_0 50
#define PRIX_N0_BON_1 200
#define PRIX_N0_BON_2 1000
#define PRIX_N0_AMEL 100000

#define PRIX_N1_BON_0 200
#define PRIX_N1_BON_1 700
#define PRIX_N1_BON_2 2000
#define PRIX_N1_AMEL 1000000

Gamebuino gb;

struct t_partie {
    int bon_0 = 0, bon_1 = 0,
	bon_2 = 0, niveau = 0;
    long argent = 100;
    int premiere_partie = 0;
} partie;

int curs = 0;

const byte kebaberie[] PROGMEM = {32,32,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7F,0xFC,0x0,0x1,0x80,0x3,0x0,0x6,0x0,0x0,0xC0,0x8,0x56,0x9D,0x20,0x10,0x66,0x95,0x10,0x20,0x54,0xDD,0x88,0x40,0x56,0xD5,0x84,0x40,0x0,0x0,0x6,0x4F,0xFF,0xFF,0xE7,0x49,0x0,0x38,0x25,0x49,0x0,0x7C,0x25,0x4B,0x86,0x6C,0x25,0x4B,0x86,0x44,0x25,0x4B,0x9E,0x54,0x25,0x4B,0x9B,0x38,0x25,0x49,0x1B,0x12,0x25,0x4F,0xFF,0xFF,0xE5,0x40,0x0,0x0,0x5,0x40,0x0,0x0,0x5,0x40,0x0,0x0,0x5,0x50,0x0,0x80,0xE5,0x50,0x0,0x80,0x45,0x51,0xF0,0x80,0x45,0x50,0x40,0x80,0x45,0x5C,0x47,0x80,0x45,0x54,0x44,0x80,0x45,0x54,0x44,0x80,0xA5,};
const byte ambulant[] PROGMEM = {32,32,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF,0xFF,0x80,0x0,0x1F,0xFF,0xC0,0xE0,0x3F,0xFF,0xE0,0xE0,0x1F,0xFF,0xC0,0xE0,0x8,0x3,0x81,0xF0,0x8,0x1,0x81,0xB0,0x1C,0x1,0x81,0x10,0x8,0x1,0x81,0x50,0x8,0x1,0x81,0xF0,0x8,0x0,0x80,0xB0,0x1C,0x0,0x80,0x90,0x1C,0x0,0x80,0x90,0x1C,0x6,0x80,0xB0,0x1C,0x6,0x80,0xB0,0x9,0xF,0x81,0xB0,0xB,0x4F,0x83,0xF0,0x1F,0xFF,0xFF,0xF0,0x10,0x0,0x4,0x90,0x15,0x69,0xD4,0xD0,0x16,0x69,0x54,0x50,0x15,0x4D,0xDC,0x50,0x15,0x6D,0x5C,0x50,0x30,0x0,0x6,0x70,0x4F,0xFF,0xFE,0x30,0x48,0x0,0x2,0xF0,0x30,0x0,0x2,0xF0,};
const byte broche[] PROGMEM = {16,12,0x40,0x40,0x64,0xC0,0x2E,0x0,0x1B,0x0,0x19,0x0,0x13,0x0,0x55,0x0,0x11,0x40,0x4E,0x0,0x14,0xC0,0x75,0xE0,0xFF,0xF0,};
const byte friteuse[] PROGMEM = {16,12,0x2,0x0,0x20,0x80,0x0,0x0,0xA6,0x0,0x34,0x0,0x3F,0xE0,0xBD,0x0,0xFF,0x0,0xBD,0x40,0x81,0x0,0x81,0x0,0x81,0x0,};
const byte kebabier[] PROGMEM = {16,12,0x1F,0x80,0x3F,0xC0,0x30,0xC0,0x29,0x40,0x20,0x40,0x22,0x40,0x27,0x40,0x10,0x80,0x3F,0xC0,0x41,0x20,0x44,0x20,0x40,0x20,};

const byte amelioration[] PROGMEM = {16,12,0x6,0x0,0xF,0x0,0x19,0x80,0x30,0xC0,0x60,0x60,0x70,0xE0,0x10,0x80,0x10,0x80,0x10,0x80,0x10,0x80,0x10,0x80,0x1F,0x80,};

void afficher();
void afficher_niveau();
void afficher_curseur();
void afficher_textes();
void jeu();
void gerer_actions();
bool acheter(int *bon, int prix);
void acheter_amel();
void gagner_argent();
long prix_bon_0();
long prix_bon_1();
long prix_bon_2();
long prix_amel();
void sauvegarder();
void restaurer();

void setup(){
    restaurer();
    if (!partie.premiere_partie)
    {
	partie.premiere_partie = 1111;
	partie.argent = 100;
    }
    gb.begin();
    gb.titleScreen(F("Master Kebab"));
}

void loop(){
  if(gb.update()){
      jeu();
  }
}

void jeu(){
    afficher();
    gerer_actions();
    gagner_argent();
}

void gagner_argent(){
    if (gb.frameCount % 20 == 0)
    {
	partie.argent += (1 + partie.bon_2) *
	    (partie.bon_0 * prix_bon_0() / 5 +
	     partie.bon_1 * prix_bon_1() / 7) / 10;
    }
}

void gerer_actions(){
    if (gb.buttons.repeat(BTN_A,20))
    {
	if (curs == 0)
	    acheter(&(partie.bon_0),prix_bon_0());;
	if (curs == 1)
	    acheter(&(partie.bon_1),prix_bon_1());
	if (curs == 2)
	    acheter(&(partie.bon_2),prix_bon_2());
	if (curs == 3)
	    acheter_amel();
	
    }
    if (gb.buttons.pressed(BTN_DOWN))
	curs = min(3,curs + 1);
    if (gb.buttons.pressed(BTN_UP))
	curs = max(0,curs - 1);
    if (gb.buttons.pressed(BTN_LEFT))
	curs = max(0, curs - 3);
    if (gb.buttons.pressed(BTN_RIGHT))
	curs = min(3, curs + 3);
    if (gb.buttons.pressed(BTN_C))
    {
	sauvegarder();
	gb.titleScreen(F("Master Kebab"));
    }
}

bool acheter(int *bon, int prix)
{
    if (partie.argent >= prix)
    {
	partie.argent -= prix;
	(*bon)++;
	return true;
    }
    return false;
}

void acheter_amel()
{
    if (acheter (&(partie.niveau), prix_amel()))
    {
	partie.bon_0 = 0;
	partie.bon_1 = 0;
	partie.bon_2 = 0;
	partie.argent = prix_amel() / 1000;
    }
    //TODO pour eviter les bugs
    if (partie.niveau > 1)
    {
	partie.argent = 100;
	partie.niveau = 0;
    }
}

long prix_bon_0(){
    int prix = 0;
    if (partie.niveau == 0)
    { prix = PRIX_N0_BON_0; }
    if (partie.niveau == 1)
    { prix = PRIX_N1_BON_0; }
    prix = prix + partie.bon_0 * prix / 8;
    return prix;
}


long prix_bon_1(){
    long prix = 0;
    if (partie.niveau == 0)
    { prix = PRIX_N0_BON_1; }
    if (partie.niveau == 1)
    { prix = PRIX_N1_BON_1; }
    prix = prix + partie.bon_1 * prix / 4;
    return prix;
}

long prix_bon_2(){
    long prix = 0;
    if (partie.niveau == 0)
    { prix = PRIX_N0_BON_2; }
    if (partie.niveau == 1)
    { prix = PRIX_N1_BON_2; }
    prix = prix + partie.bon_2 * prix / 4;
    return prix;
}

long prix_amel(){
    if (partie.niveau == 0)
	return PRIX_N0_AMEL;
    if (partie.niveau == 1)
	return PRIX_N1_AMEL;
    else return 0;
}


void afficher(){
    afficher_niveau();
    afficher_curseur();
    afficher_textes();
}

void afficher_textes(){
    // BONUS
    gb.display.cursorX = POS_X_BON + LARG_CURS;
    gb.display.cursorY = POS_Y_BON_0;
    gb.display.print(prix_bon_0());
    gb.display.println("$");
    gb.display.println(partie.bon_0);
    gb.display.cursorX = POS_X_BON + LARG_CURS;
    gb.display.cursorY = POS_Y_BON_1;
    gb.display.print(prix_bon_1());
    gb.display.println("$");
    gb.display.println(partie.bon_1);
    gb.display.cursorX = POS_X_BON + LARG_CURS;
    gb.display.cursorY = POS_Y_BON_2;
    gb.display.print(prix_bon_2());
    gb.display.println("$");
    gb.display.println(partie.bon_2);
    //AMEL
    gb.display.cursorY = POS_Y_AMEL + LARG_CURS;
    gb.display.cursorX = POS_X_AMEL - 16;
    gb.display.print(prix_amel());
    gb.display.println("$");
    // SOUSOUS
    gb.display.cursorX = 30;
    gb.display.cursorY = 0;
    gb.display.print("$:");
    gb.display.println(partie.argent);
    
}

void afficher_curseur(){
    int pos_x = 2, pos_y = 2;
    if (curs <= 2)
	pos_x = POS_X_BON;
    else
	pos_x = POS_X_AMEL;
    switch (curs){
    case 0: case 3:
	pos_y = POS_Y_BON_0;
	break;
    case 1:
	pos_y = POS_Y_BON_1;
	break;
    case 2:
	pos_y = POS_Y_BON_2;
	break;
    }
    gb.display.drawRect(pos_x, pos_y, LARG_CURS , LARG_CURS);
}

void afficher_niveau(){
    if (partie.niveau == 0) {
	gb.display.drawBitmap(POS_X_BON, POS_Y_BON_0, friteuse);
	gb.display.drawBitmap(POS_X_BON, POS_Y_BON_1, broche);
	gb.display.drawBitmap(POS_X_BON, POS_Y_BON_2, kebabier);
	gb.display.drawBitmap(POS_X_ART, POS_Y_ART, ambulant);
    }
    if (partie.niveau == 1) {
	gb.display.drawBitmap(POS_X_BON, POS_Y_BON_0, friteuse);
	gb.display.drawBitmap(POS_X_BON, POS_Y_BON_1, broche);
	gb.display.drawBitmap(POS_X_BON, POS_Y_BON_2, kebabier);
	gb.display.drawBitmap(POS_X_ART, POS_Y_ART, kebaberie);
    }
    gb.display.drawBitmap(POS_X_AMEL, POS_Y_AMEL, amelioration);

}

void sauvegarder(void)
{
    for (unsigned i = 0; i<sizeof(partie); ++i)
        EEPROM.write(i,((uint8_t*)&partie)[i]);
}

void restaurer(void)
{
    for (unsigned i=0; i<sizeof(partie); ++i)
        ((uint8_t*)&partie)[i] = EEPROM.read(i);
}