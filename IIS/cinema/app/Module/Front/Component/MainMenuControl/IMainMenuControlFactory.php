<?php declare(strict_types = 1);

namespace App\Module\Front\Component\MainMenuControl;

/**
 */
interface IMainMenuControlFactory
{
    public function create(): MainMenuControl;
}
