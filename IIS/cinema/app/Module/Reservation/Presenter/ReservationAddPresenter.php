<?php declare(strict_types=1);

namespace App\Module\Reservation\Presenter;

use App\Module\Content\Entity\Event;
use App\Module\Core\Security\User;
use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\Reservation\Component\EventSummaryControl\EventSummaryControl;
use App\Module\Reservation\Component\EventSummaryControl\IEventSummaryControlFactory;
use App\Module\Reservation\Component\ReservationFormControl\IReservationFormControlFactory;
use App\Module\Reservation\Component\ReservationFormControl\ReservationFormControl;
use App\Module\Reservation\Entity\Reservation;

/**
 */
class ReservationAddPresenter extends AbstractFrontPresenter
{
    /** @var IEventSummaryControlFactory @inject */
    public $eventSummaryControlFactory;

    /** @var IReservationFormControlFactory @inject */
    public $reservationFormControlFactory;

    /** @var User @inject */
    public $user;

    /** @var Event */
    private $event;

    public function actionDefault(Event $entity): void
    {
        $this->event = $entity;
    }

    protected function createComponentEventSummary(): EventSummaryControl
    {
        return $this->eventSummaryControlFactory->create($this->event);
    }

    protected function createComponentReservationForm(): ReservationFormControl
    {
        $userEntity = $this->user->isLoggedIn() ? $this->user->getEntity() : null;
        $control = $this->reservationFormControlFactory->create($this->event, $userEntity);
        $control->onSuccess[] = function (Reservation $reservation) {
            $this->flashMessage('Rezervace byla vytvořena.');
            $this->redirect(':Reservation:ReservationDetail:default', $reservation);
        };

        return $control;
    }
}
