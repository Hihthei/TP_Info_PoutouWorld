#include "LevelHeader.h"
#include "LevelScene.h"
#include "Image.h"

LevelHeader::LevelHeader(LevelScene &scene):
    UIObject(scene), m_levelScene(scene), m_fireflyCount(nullptr), m_heartCount(nullptr), m_lifeCount(nullptr),
    m_lifeCountInt(3), m_heartCountInt(3)
{
    m_name = "LevelHeader";

    AssetManager &assets = scene.GetAssetManager();
    TTF_Font *font = assets.GetFont(FontID::LARGE);
    SDL_Color color = assets.GetColor(ColorID::NORMAL);

    int w, h;
    TTF_SizeUTF8(font, u8"00", &w, &h);
    float imgH = 80.0f;
    float imgW = imgH;
    float numW = (float)w;
    float sep = 20.0f;
    float panelW = imgW + sep + numW;
    float panelH = imgH;

    m_rect.anchorMin.Set(0.0f, 0.0f);
    m_rect.anchorMax.Set(0.0f, 0.0f);
    m_rect.offsetMin.Set(80.0f, 40.0f);
    m_rect.offsetMax = m_rect.offsetMin + Vec2(panelW, panelH);

    RE_Atlas *atlas = assets.GetAtlas(AtlasID::UI);

    float currX = 0.0f;
    float currY = 0.0f;


    // Image du nombre de vies___________________________________________________________________

    //TODO -> la boucle for qui marche pas

    /*

    for (int i = 0; i < m_lifeCountInt; i++)
    {
        RE_AtlasPart* partLife = atlas->GetPart("Life");
        AssertNew(partLife);
        Image* lifeImage = new Image(scene, partLife, 0);
        lifeImage->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
        lifeImage->GetLocalRect().anchorMax.Set(0.0f, 0.0f);
        lifeImage->GetLocalRect().offsetMin.Set(currX, currY);
        lifeImage->GetLocalRect().offsetMax.Set(currX + imgW, currY + imgH);
        lifeImage->SetParent(this);

        currX += imgW + sep;
    } */

    RE_AtlasPart* partLife = atlas->GetPart("Life");
    AssertNew(partLife);
    Image* lifeImage = new Image(scene, partLife, 0);
    lifeImage->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
    lifeImage->GetLocalRect().anchorMax.Set(0.0f, 0.0f);
    lifeImage->GetLocalRect().offsetMin.Set(currX, currY);
    lifeImage->GetLocalRect().offsetMax.Set(currX + imgW, currY + imgH);
    lifeImage->SetParent(this);

    currX += imgW + sep;
    

//___________________________________________________________________________________//
//     Il faut enlever cette partie pour plus afficher les nombres
 
 
    // Compteur du nombre de vies
    m_lifeCount = new Text(scene, "3", font, color);
    //TODO -> effacer le texte
    m_lifeCount->SetAnchor(RE_Anchor::WEST);
    m_lifeCount->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
    m_lifeCount->GetLocalRect().anchorMax.Set(0.0f, 0.0f);
    m_lifeCount->GetLocalRect().offsetMin.Set(currX, currY);
    m_lifeCount->GetLocalRect().offsetMax.Set(currX + numW, currY + imgH);
    m_lifeCount->SetParent(this); 
//___________________________________________________________________________________//

    //Mise à jour de la position pour les prochains éléments

    currX = 0;
    currY += imgH + sep + 3.0f;


    // Image du nombre de coeurs___________________________________________________________________

/*    for (int i = 0; i < m_lifeCountInt; i++)
    {
        RE_AtlasPart* partHeart = atlas->GetPart("Heart");
        AssertNew(partHeart);
        Image* heartImage = new Image(scene, partHeart, 0);
        heartImage->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
        heartImage->GetLocalRect().anchorMax.Set(0.0f, 0.0f);
        heartImage->GetLocalRect().offsetMin.Set(currX, currY);
        heartImage->GetLocalRect().offsetMax.Set(currX + imgW, currY + imgH);
        heartImage->SetParent(this);

        currX += imgW + sep;
    } */
    
    RE_AtlasPart* partHeart = atlas->GetPart("Heart");
    AssertNew(partHeart);
    Image* heartImage = new Image(scene, partHeart, 0);
    heartImage->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
    heartImage->GetLocalRect().anchorMax.Set(0.0f, 0.0f);
    heartImage->GetLocalRect().offsetMin.Set(currX, currY);
    heartImage->GetLocalRect().offsetMax.Set(currX + imgW, currY + imgH);
    heartImage->SetParent(this);

    currX += imgW + sep;


//___________________________________________________________________________________//
//     Il faut enlever cette partie pour plus afficher les nombres

    // Compteur du nombre de coeurs
    m_heartCount = new Text(scene, "3", font, color);
    m_heartCount->SetAnchor(RE_Anchor::WEST);
    m_heartCount->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
    m_heartCount->GetLocalRect().anchorMax.Set(0.0f, 0.0f);
    m_heartCount->GetLocalRect().offsetMin.Set(currX, currY);
    m_heartCount->GetLocalRect().offsetMax.Set(currX + numW, currY + imgH);
    m_heartCount->SetParent(this);

//___________________________________________________________________________________//

    //Mise à jour de la position pour les prochains éléments

    currX = 0;
    currY += imgH + sep + 3.0f;

    // Image du nombre de luciolles___________________________________________________________________
    RE_AtlasPart *partFirefly = atlas->GetPart("Firefly");
    AssertNew(partFirefly);
    Image *fireflyImage = new Image(scene, partFirefly, 0);
    fireflyImage->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
    fireflyImage->GetLocalRect().anchorMax.Set(0.0f, 0.0f);
    fireflyImage->GetLocalRect().offsetMin.Set(currX, currY);
    fireflyImage->GetLocalRect().offsetMax.Set(currX + imgW, currY + imgH);
    fireflyImage->SetParent(this);

    currX += imgW + sep;

    
    // Compteur du nombre de luciolles
    m_fireflyCount = new Text(scene, "0", font, color);
    m_fireflyCount->SetAnchor(RE_Anchor::WEST);
    m_fireflyCount->GetLocalRect().anchorMin.Set(0.0f, 0.0f);
    m_fireflyCount->GetLocalRect().anchorMax.Set(0.0f, 0.0f);
    m_fireflyCount->GetLocalRect().offsetMin.Set(currX, currY);
    m_fireflyCount->GetLocalRect().offsetMax.Set(currX + numW, currY + imgH);
    m_fireflyCount->SetParent(this);
}

void LevelHeader::Update()
{
    Player *player = m_levelScene.GetPlayer();

    

    m_fireflyCount->SetString(std::to_string(player->GetFireflyCount()));

    m_lifeCount->SetString(std::to_string(player->GetLifeCount()));

    m_heartCount->SetString(std::to_string(player->GetHeartCount()));

//    m_lifeCountInt = player->GetLifeCount();

//    m_heartCountInt = player->GetHeartCount();
}
