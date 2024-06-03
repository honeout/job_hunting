#include <imgui.h>
#include "AfterimageManager.h"
#include "Collision.h"




// XVˆ—
void AfterimageManager::Update(float elapsedTime)
{
    for (Afterimage* after : afterimages)
    {
        after->Update(elapsedTime);
    }


    // ”jŠüˆ— –ˆƒtƒŒ[ƒ€‚±‚±‚Åˆê‹C‚ÉÁ‚·B
    for (Afterimage* after : removes)
    {
        std::vector<Afterimage*>::iterator it = std::find(afterimages.begin(), afterimages.end(),
            after);

        // Œ©‚Â‚©‚Á‚½‚ç
        if (it != afterimages.end())
        {
            // íœ“o˜^
            afterimages.erase(it);

        }

        // Á‚¦‚½–ó‚¶‚á‚È‚¢‚Ì‚Å
        // “G‚Ì”jŠüˆ—
        delete after;// “ü‚ê•¨”jŠü

    }
    // ”jŠüƒŠƒXƒg‚ğƒNƒŠƒA
    removes.clear();

    //// “G“¯m‚ÌÕ“Ëˆ—
    //CollisiionEnemyVsEnemies();
}

// “–‚½‚è”»’è•`‰æ
void AfterimageManager::DrawDebugPrimitive()
{
   
    for (Afterimage* after : afterimages)
    {
        after->DrawDebugPrimitive();
    }
}


// •`‰æˆ—
void AfterimageManager::Render(RenderContext& context, ModelShader* shader)
{
    for (Afterimage* after : afterimages)
    {
        after->Render(context, shader);
    }
}

void AfterimageManager::Remove(Afterimage* after)
{
    // íœİ’è
    removes.insert(after);// ‚¢‚«‚È‚èÁ‚·‚ÆƒoƒO‚é‚½‚ß
  
}




void AfterimageManager::Register(Afterimage* after)
{
    // ì‚Á‚½‚çŒã‚ë‚©‚ç’Ç‰Á‚µ‚Ä‚¢‚­‘‚¦‚Ä‚¢‚­‚¾‚¯
    afterimages.emplace_back(after);
}
//
//void AfterimageManager::CollisiionEnemyVsEnemies()
//{
//    // Å‘å”
//    size_t playerafterimageCount = playerafterimages.size();
//    for (int i = 0; i < playerafterimageCount; ++i) {
//        // i”Ô–Ú‚ÌˆÊ’u
//        Enemy* enemyA = enemies.at(i);
//        for (int j = i + 1; j < enemyCount; ++j)
//        {
//            Enemy* enemyB = enemies.at(j);
//            DirectX::XMFLOAT3 outPosition;
//            if (Collision::IntersectSpherVsSphere(
//                enemyA->GetPosition(),
//                enemyA->GetRadius(),
//                enemyB->GetPosition(),
//                enemyB->GetRadius(),
//                outPosition))
//            {
//                enemyB->SetPosition(outPosition);
//            }
//        }
//
//    }
//
//}



void AfterimageManager::DrawDebugGUI()
{
}

// ƒGƒlƒ~[‘Síœ
void AfterimageManager::Clear()
{
    for (Afterimage* after : afterimages)
    {
        // À‘Ì‚ğÁ‚µ‚½ŠÇ—‚µ‚Ä‚¢‚é”‚Í‚»‚Ì‚Ü‚Ü
        delete after;

    }
    // vector‚Ì”‚ğ‚O‚É
    afterimages.clear();
}
