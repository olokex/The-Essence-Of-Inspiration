<?php declare(strict_types = 1);

namespace App\Module\User;

use App\Module\Core\Extension\BaseExtension;
use App\Module\Core\Extension\IRouteProvider;
use App\Module\Core\Route\EntityIdRouteFactory;
use App\Module\User\Entity\User;
use Doctrine\ORM\EntityManagerInterface;
use Nette\Application\Routers\Route;
use Nette\Application\Routers\RouteList;
use Nettrine\ORM\DI\Traits\TEntityMapping;

/**
 */
class UserExtension extends BaseExtension implements IRouteProvider
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

        $userRepository = $this->entityManager->getRepository(User::class);
        $routeList[] = $this->entityIdRouteFactory->create($userRepository, '/setting/<entity>', [
            'presenter' => 'Setting',
            'action' => 'default',
        ]);

        $routeList[] = new Route('/user-list', [
            'presenter' => 'UserList',
            'action' => 'default',
        ]);

        $routeList[] = new Route('/login', [
            'presenter' => 'Login',
            'action' => 'default',
        ]);

        $routeList[] = new Route('/registration', [
            'presenter' => 'Registration',
            'action' => 'default',
        ]);

        $routeList[] = new Route('/logout', [
            'presenter' => 'Logout',
            'action' => 'default',
        ]);

        return $routeList;
    }
}
