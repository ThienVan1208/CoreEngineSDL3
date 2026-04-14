content = open('scripts/src/player.cpp', 'r', encoding='utf-8').read()
idx = content.find('void Player::OnCollisionEnter')
if idx != -1:
    new_content = content[:idx] + '''void Player::OnCollisionEnter(Collider *other)
{
    if (other->GetObject()->tag == "Enemy")
    {
        if (myHealth)
        {
            myHealth->TakeDamage(other->GetObject()->contactDamage);
            SDL_Log("Player touched Enemy! Took %f damage. Current HP: %f", other->GetObject()->contactDamage, myHealth->currentHp);
        }
    }
}
'''
    open('scripts/src/player.cpp', 'w', encoding='utf-8').write(new_content)
    print('replaced successfully')
else:
    print('not found')
