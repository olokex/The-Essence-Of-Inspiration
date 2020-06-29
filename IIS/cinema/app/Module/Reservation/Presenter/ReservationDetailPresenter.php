<?php declare(strict_types = 1);

namespace App\Module\Reservation\Presenter;

use App\Module\Core\Security\User;
use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\Reservation\Component\ContactInfoSummaryControl\ContactInfoSummaryControl;
use App\Module\Reservation\Component\ContactInfoSummaryControl\IContactInfoSummaryControlFactory;
use App\Module\Reservation\Component\EventSummaryControl\EventSummaryControl;
use App\Module\Reservation\Component\EventSummaryControl\IEventSummaryControlFactory;
use App\Module\Reservation\Component\TicketSummaryControl\ITicketSummaryControlFactory;
use App\Module\Reservation\Component\TicketSummaryControl\TicketSummaryControl;
use App\Module\Reservation\Entity\Reservation;
use App\Module\User\Service\AccessGuard;

/**
 */
class ReservationDetailPresenter extends AbstractFrontPresenter
{
    /** @var IEventSummaryControlFactory @inject */
    public $eventSummaryControlFactory;

    /** @var ITicketSummaryControlFactory @inject */
    public $ticketSummaryControlFactory;

    /** @var IContactInfoSummaryControlFactory @inject */
    public $contactInfoSummaryControlFactory;

    /** @var User @inject */
    public $user;

    /** @var Reservation */
    private $reservation;

    public function actionDefault(Reservation $entity): void
    {
        $this->reservation = $entity;

        if (false === $this->canAccess()) {
            $this->redirect(':Homepage:Homepage:default');
        }
    }

    private function canAccess(): bool
    {
        return $this->user->isLoggedIn() &&
            ($this->user->getEntity() === $this->reservation->getUser() ||
                AccessGuard::hasCashierAccess($this->user->getEntity()));
    }

    protected function createComponentEventSummary(): EventSummaryControl
    {
        return $this->eventSummaryControlFactory->create($this->reservation->getEvent());
    }

    protected function createComponentTicketSummary(): TicketSummaryControl
    {
        return $this->ticketSummaryControlFactory->create($this->reservation);
    }

    protected function createComponentContactInfoSummary(): ContactInfoSummaryControl
    {
        return $this->contactInfoSummaryControlFactory->create($this->reservation);
    }
}
