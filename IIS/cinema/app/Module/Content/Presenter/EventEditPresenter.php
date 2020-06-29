<?php declare(strict_types = 1);

namespace App\Module\Content\Presenter;

use App\Module\Content\Component\EventFormControl\EventFormControl;
use App\Module\Content\Component\EventFormControl\IEventFormControlFactory;
use App\Module\Content\Container\EventContainer\EventData;
use App\Module\Content\Entity\Event;
use App\Module\Content\Service\EventService;
use App\Module\Core\Security\User;
use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\User\Service\AccessGuard;

/**
 */
class EventEditPresenter extends AbstractFrontPresenter
{
    /** @var EventService @inject */
    public $eventService;

    /** @var IEventFormControlFactory @inject */
    public $eventFormControlFactory;

    /** @var User @inject */
    public $user;

    /** @var Event */
    private $event;

    public function startup(): void
    {
        parent::startup();

        if (false === $this->user->isLoggedIn() || false === AccessGuard::hasEditorAccess($this->user->getEntity())) {
            $this->redirect(':Homepage:Homepage:default');
        }
    }

    public function actionDefault(Event $entity): void
    {
        $this->event = $entity;
    }

    protected function createComponentEventForm(): EventFormControl
    {
        $control = $this->eventFormControlFactory->create('Upravit', $this->event);
        $control->onSuccess[] = function (EventData $eventData) {
            $this->eventService->updateExistingEvent($this->event, $eventData);
            $this->flashMessage('Událost byla aktualizována.');
            $this->redirect(':Content:ContentDetail:default', $this->event->getContent());
        };

        return $control;
    }
}
