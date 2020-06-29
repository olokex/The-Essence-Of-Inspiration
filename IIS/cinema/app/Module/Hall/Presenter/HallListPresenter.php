<?php declare(strict_types=1);

namespace App\Module\Hall\Presenter;

use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\Hall\Factory\HallListFactory;
use App\Module\Hall\Repository\HallRepository;
use App\Module\User\Service\AccessGuard;
use App\Module\Core\Security\User;
use Ublaboo\DataGrid\DataGrid;

/**
 */
class HallListPresenter extends AbstractFrontPresenter
{
    /** @var HallRepository @inject */
    public $hallRepository;

    /** @var HallListFactory @inject */
    public $hallListFactory;

    /** @var User @inject */
    public $user;

    public function startup(): void
    {
        parent::startup();

        if (false === $this->user->isLoggedIn() || false === AccessGuard::hasEditorAccess($this->user->getEntity())) {
            $this->redirect(':Homepage:Homepage:default');
        }
    }

    protected function createComponentHallList(): DataGrid
    {
        $control = $this->hallListFactory->create();

        $control->getInlineAdd()->onSubmit[] = function() {
            $this->flashMessage('Sál byl přidán.');
            $this->redirect('this');
        };
        $control->getInlineEdit()->onSubmit[] = function() {
            $this->flashMessage('Sál byl aktualizován.');
            $this->redirect('this');
        };

        return $control;
    }

    public function handleEdit(string $id): void
    {
        $hall = $this->hallRepository->getById((int) $id);
        $this->redirect(':Hall:HallEdit:default', $hall);
    }
}
