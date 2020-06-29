<?php declare(strict_types = 1);

namespace App\Module\Content;

use App\Module\Content\Entity\Content;
use App\Module\Content\Entity\Event;
use App\Module\Core\Extension\BaseExtension;
use App\Module\Core\Extension\IRouteProvider;
use App\Module\Core\Route\EntityIdRouteFactory;
use Doctrine\ORM\EntityManagerInterface;
use Nette\Application\Routers\Route;
use Nette\Application\Routers\RouteList;
use Nettrine\ORM\DI\Traits\TEntityMapping;

/**
 */
class ContentExtension extends BaseExtension implements IRouteProvider
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

        $routeList[] = new Route('/content-list', [
            'presenter' => 'ContentList',
            'action' => 'default',
        ]);

        $contentRepository = $this->entityManager->getRepository(Content::class);
        $routeList[] = $this->entityIdRouteFactory->create($contentRepository, '/content-edit[/<entity>]', [
            'presenter' => 'ContentEdit',
            'action' => 'default',
        ]);

        $routeList[] = $this->entityIdRouteFactory->create($contentRepository, '/content-detail/<entity>', [
            'presenter' => 'ContentDetail',
            'action' => 'default',
        ]);

        $routeList[] = $this->entityIdRouteFactory->create($contentRepository, '/event-add/<entity>', [
            'presenter' => 'EventAdd',
            'action' => 'default',
        ]);

        $eventRepository = $this->entityManager->getRepository(Event::class);
        $routeList[] = $this->entityIdRouteFactory->create($eventRepository, '/event-edit[/<entity>]', [
            'presenter' => 'EventEdit',
            'action' => 'default',
        ]);

        return $routeList;
    }
}
