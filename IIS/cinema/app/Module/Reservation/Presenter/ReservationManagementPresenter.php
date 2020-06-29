<?php declare(strict_types = 1);

namespace App\Module\Reservation\Presenter;

use App\Module\Core\Security\User;
use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\Reservation\Enum\ReservationStateType;
use App\Module\Reservation\Factory\ReservationListFactory;
use App\Module\Reservation\Repository\ReservationRepository;
use App\Module\Reservation\Service\ReservationService;
use App\Module\User\Repository\UserRepository;
use App\Module\User\Service\AccessGuard;
use Ublaboo\DataGrid\DataGrid;

/**
 */
class ReservationManagementPresenter extends AbstractFrontPresenter
{
    /** @var User @inject */
    public $user;

    /** @var ReservationRepository @inject */
    public $reservationRepository;

    /** @var UserRepository @inject */
    public $userRepository;

    /** @var ReservationListFactory @inject */
    public $reservationListFactory;

    /** @var ReservationService @inject */
    public $reservationService;

    protected function startup(): void
    {
        parent::startup();

        if (false === $this->user->isLoggedIn() || false === AccessGuard::hasCashierAccess($this->user->getEntity())) {
            $this->redirect(':Homepage:Homepage:default');
        }
    }

    protected function createComponentReservationList(): DataGrid
    {
        return $this->reservationListFactory->create(null, true);
    }

    public function handleDetail(string $id): void
    {
        $reservation = $this->reservationRepository->getById((int) $id);
        $this->redirect(':Reservation:ReservationDetail:default', $reservation);
    }

    public function handlePay(string $id): void
    {
        $reservation = $this->reservationRepository->getById((int) $id);

        if (AccessGuard::hasCashierAccess($this->user->getEntity())) {
            $this->reservationService->changeReservationState($reservation, ReservationStateType::PAID());
        }
        $this->redirect('this');
    }

    public function handleRemove(string $id): void
    {
        $reservation = $this->reservationRepository->getById((int) $id);

        if (AccessGuard::hasCashierAccess($this->user->getEntity())) {
            $this->reservationService->removeReservation($reservation);
        }
        $this->redirect('this');
    }
}
