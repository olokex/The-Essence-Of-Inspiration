<?php declare(strict_types = 1);

namespace App\Module\Homepage;

use Nette\Application\Routers\Route;
use Nette\Application\Routers\RouteList;
use App\Module\Core\Extension\BaseExtension;
use App\Module\Core\Extension\IRouteProvider;

/**
 */
class HomepageExtension extends BaseExtension implements IRouteProvider
{
    public function getRouteDefinition(): RouteList
    {
        $routeList = new RouteList($this->getModuleName());

        $routeList[] = new Route('/', [
            'presenter' => 'Homepage',
            'action' => 'default',
        ]);

        return $routeList;
    }
}
