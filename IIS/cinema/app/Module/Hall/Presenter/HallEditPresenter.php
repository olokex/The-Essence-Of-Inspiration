<?php declare(strict_types = 1);

namespace App\Module\Hall\Presenter;

use App\Module\Core\Security\User;
use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\Hall\Component\HallEditFormControl\HallEditFormControl;
use App\Module\Hall\Component\HallEditFormControl\IHallEditFormControlFactory;
use App\Module\Hall\Entity\Hall;
use App\Module\User\Service\AccessGuard;

/**
 */
class HallEditPresenter extends AbstractFrontPresenter
{
    /** @var IHallEditFormControlFactory @inject */
    public $hallEditFormControlFactory;

    /** @var User @inject */
    public $user;

    /** @var Hall|null */
    private $hall;

    public function actionDefault(?Hall $entity = null): void
    {
        $this->hall = $entity;

        if (false === $this->user->isLoggedIn() || false === AccessGuard::hasEditorAccess($this->user->getEntity())) {
            $this->redirect(':Homepage:Homepage:default');
        }
    }

    public function renderDefault(): void
    {
        $this->template->hall = $this->hall;
    }

    protected function createComponentHallEditForm(): HallEditFormControl
    {
        $control = $this->hallEditFormControlFactory->create($this->hall);
        $control->onSuccess[] = function() {
            $this->flashMessage('Informace byly uloženy.');
            $this->redirect(':Hall:HallList:default');
        };

        return $control;
    }
}
