<?php declare(strict_types = 1);

namespace App\Module\Reservation\Presenter;

use App\Module\Core\Security\User;
use App\Module\Front\Presenter\AbstractFrontPresenter;
use App\Module\Reservation\Factory\ReservationListFactory;
use App\Module\Reservation\Repository\ReservationRepository;
use App\Module\User\Entity\User as UserEntity;
use Ublaboo\DataGrid\DataGrid;

/**
 * Presenter for displaying user's reservations
 */
class ReservationListPresenter extends AbstractFrontPresenter
{
    /** @var ReservationRepository @inject */
    public $reservationRepository;

    /** @var ReservationListFactory @inject */
    public $reservationListFactory;

    /** @var User @inject */
    public $user;

    /** @var UserEntity */
    private $userEntity;

    public function startup(): void
    {
        parent::startup();

        if (false === $this->user->isLoggedIn()) {
            $this->redirect(':Homepage:Homepage:default');
        }
    }

    public function actionDefault(UserEntity $entity): void
    {
        $this->userEntity = $entity;
    }

    protected function createComponentReservationList(): DataGrid
    {
        return $this->reservationListFactory->create($this->userEntity);
    }

    public function handleDetail(string $id): void
    {
        $reservation = $this->reservationRepository->getById((int) $id);
        $this->redirect(':Reservation:ReservationDetail:default', $reservation);
    }
}
