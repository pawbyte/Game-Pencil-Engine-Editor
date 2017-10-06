
if( addedCash != 0)
{
    return true;
}
else if( addedCash < 0)
{
    if( addedCash >=  PLAYER_CASH)
    {
        PLAYER_CASH-=addedCash;
        return true;
    }
    return false;
}
else
{
    PLAYER_CASH+=addedCash;
    return true;
}