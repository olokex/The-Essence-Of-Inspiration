<?php declare(strict_types = 1);

namespace App\Module\Content\Service;

use App\Module\Content\Entity\Content;
use Nette\Utils\Strings;
use WebChemistry\Images\IImageStorage;
use WebChemistry\Images\Resources\Transfer\UploadResource;

/**
 */
class ImageService
{
    /** @var IImageStorage */
    private $imageStorage;

    public function __construct(IImageStorage $imageStorage)
    {
        $this->imageStorage = $imageStorage;
    }

    public function insertImage(Content $content, UploadResource $resource): string
    {
        if ($content->getImage()) {
            $oldResource = $this->imageStorage->createResource($content->getImage());
            $this->imageStorage->delete($oldResource);
        }

        $name = $this->getImageName($content, $resource);
        $resource->setName($name);
        $fileResource = $this->imageStorage->save($resource);

        return $fileResource->getId();
    }

    private function getImageName(Content $content, UploadResource $resource): string
    {
        $fileExtension = \pathinfo($resource->getName(), \PATHINFO_EXTENSION);

        return Strings::webalize($content->getTitle() . '-' . $content->getId()) . '.' . $fileExtension;
    }
}
