<?php declare(strict_types = 1);

namespace App\Module\Core\Route;

use Doctrine\Common\Persistence\ObjectRepository;
use Nette\Application\Routers\Route;
use Package\Core\Entity\IEntity;

/**
 */
class EntityIdRouteFactory
{
    public function create(ObjectRepository $repository, $mask, $metadata = [], $flags = 0): Route
    {
        $metadata['entity'] = [
            Route::FILTER_IN => static function(string $id) use ($repository) {
                return $repository->findOneBy(['id' => (int) $id]);
            },
            Route::FILTER_OUT => static function(IEntity $entity) {
                return $entity->getId();
            },
        ];

        return new Route($mask, $metadata, $flags);
    }
}
