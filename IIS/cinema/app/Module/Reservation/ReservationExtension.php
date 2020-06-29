<?php declare(strict_types = 1);

namespace App\Module\Reservation;

use App\Module\Content\Entity\Event;
use App\Module\Core\Extension\BaseExtension;
use App\Module\Core\Extension\IRouteProvider;
use App\Module\Core\Route\EntityIdRouteFactory;
use App\Module\Reservation\Entity\Reservation;
use App\Module\User\Entity\User;
use Doctrine\ORM\EntityManagerInterface;
use Nette\Application\Routers\Route;
use Nette\Application\Routers\RouteList;
use Nettrine\ORM\DI\Traits\TEntityMapping;

/**
 */
class ReservationExtension extends BaseExtension implements IRouteProvider
{
    use TEntityMapping;

    /** @var EntityManagerInterface @inject */
    public $entityManager;

    /** @var EntityIdRouteFactory @inject */
    public $entityIdRouteFactory;

    public function loadConfiguration(): void
    {
        parent::loadConfiguration();
        $this->setEntityMappings([$this->dir . '/Entity']);
    }

    public function getRouteDefinition(): RouteList
    {
        $routeList = new RouteList($this->getModuleName());

        $eventRepository = $this->entityManager->getRepository(Event::class);
        $routeList[] = $this->entityIdRouteFactory->create($eventRepository, '/reservation-add/<entity>', [
            'presenter' => 'ReservationAdd',
            'action' => 'default',
        ]);

        $reservationRepository = $this->entityManager->getRepository(Reservation::class);
        $routeList[] = $this->entityIdRouteFactory->create($reservationRepository, '/reservation-detail/<entity>', [
            'presenter' => 'ReservationDetail',
            'action' => 'default',
        ]);

        $userRepository = $this->entityManager->getRepository(User::class);
        $routeList[] = $this->entityIdRouteFactory->create($userRepository, '/reservation-list/<entity>', [
            'presenter' => 'ReservationList',
            'action' => 'default',
        ]);

        $routeList[] = new Route('/reservation-management', [
            'presenter' => 'ReservationManagement',
            'action' => 'default',
        ]);

        return $routeList;
    }
}
