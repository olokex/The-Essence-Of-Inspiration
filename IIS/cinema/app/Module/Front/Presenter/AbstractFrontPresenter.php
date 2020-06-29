<?php declare(strict_types=1);

namespace App\Module\Front\Presenter;

use App\Module\Core\Presenter\AbstractPresenter;
use App\Module\Front\Component\MainMenuControl\IMainMenuControlFactory;
use App\Module\Front\Component\MainMenuControl\MainMenuControl;

/**
 */
abstract class AbstractFrontPresenter extends AbstractPresenter
{
    /** @var IMainMenuControlFactory @inject */
    public $mainMenuControlFactory;

    protected function createComponentMainMenu(): MainMenuControl
    {
        return $this->mainMenuControlFactory->create();
    }
}
