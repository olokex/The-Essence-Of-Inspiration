<?php declare(strict_types = 1);

namespace App\Module\Content\Presenter;

use App\Module\Content\Entity\Content;
use App\Module\Content\Factory\EventListFactory;
use App\Module\Content\Repository\EventRepository;
use App\Module\Core\Security\User;
use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\User\Service\AccessGuard;
use Ublaboo\DataGrid\DataGrid;
use WebChemistry\Images\IImageStorage;

/**
 */
class ContentDetailPresenter extends AbstractFrontPresenter
{
    /** @var EventListFactory @inject */
    public $eventListFactory;

    /** @var EventRepository @inject */
    public $eventRepository;

    /** @var IImageStorage @inject */
    public $imageStorage;

    /** @var User @inject */
    public $user;

    /** @var Content */
    private $content;

    public function actionDefault(Content $entity): void
    {
        $this->content = $entity;
    }

    public function renderDefault(): void
    {
        $this->template->content = $this->content;
        $this->template->canEdit = $this->user->isLoggedIn() && AccessGuard::hasEditorAccess($this->user->getEntity());
        $this->template->image = $this->content->getImage() ? $this->imageStorage->createResource($this->content->getImage())->getId() : null;
    }

    protected function createComponentEventList(): DataGrid
    {
        return $this->eventListFactory->create($this->content);
    }

    public function handleEditEvent(string $id): void
    {
        $event = $this->eventRepository->getById((int) $id);
        $this->redirect(':Content:EventEdit:default', $event);
    }

    public function handleReserveEvent(string $id): void
    {
        $event = $this->eventRepository->getById((int) $id);
        $this->redirect(':Reservation:ReservationAdd:default', $event);
    }
}
