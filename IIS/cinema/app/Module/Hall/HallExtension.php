<?php declare(strict_types = 1);

namespace App\Module\Hall;

use App\Module\Core\Extension\BaseExtension;
use App\Module\Core\Extension\IRouteProvider;
use App\Module\Core\Route\EntityIdRouteFactory;
use App\Module\Hall\Entity\Hall;
use Doctrine\ORM\EntityManagerInterface;
use Nette\Application\Routers\Route;
use Nette\Application\Routers\RouteList;
use Nettrine\ORM\DI\Traits\TEntityMapping;

/**
 */
class HallExtension extends BaseExtension implements IRouteProvider
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

        $routeList[] = new Route('/hall-list', [
            'presenter' => 'HallList',
            'action' => 'default',
        ]);

        $hallRepository = $this->entityManager->getRepository(Hall::class);
        $routeList[] = $this->entityIdRouteFactory->create($hallRepository, '/hall-edit[/<entity>]', [
            'presenter' => 'HallEdit',
            'action' => 'default',
        ]);

        return $routeList;
    }
}
