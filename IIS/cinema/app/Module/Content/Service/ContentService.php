<?php declare(strict_types = 1);

namespace App\Module\Content\Service;

use App\Module\Content\Container\ContentContainer\ContentData;
use App\Module\Content\Entity\Content;
use Doctrine\ORM\EntityManagerInterface;

/**
 */
class ContentService
{
    /** @var EntityManagerInterface */
    private $entityManager;

    /** @var ImageService */
    private $imageService;

    public function __construct(EntityManagerInterface $entityManager, ImageService $imageService)
    {
        $this->entityManager = $entityManager;
        $this->imageService = $imageService;
    }

    public function addNewContent(ContentData $data): Content
    {
        $content = new Content(
            $data->getTitle(),
            $data->getDescription(),
            $data->getType(),
            $data->getGenres(),
            $data->getRating(),
            $data->getLength(),
            $data->getPerformers()
        );
        $this->entityManager->persist($content);
        $this->entityManager->flush();

        if ($data->getUploadResource()) {
            $path = $this->imageService->insertImage($content, $data->getUploadResource());
            $content->setImage($path);
        }

        $this->entityManager->persist($content);
        $this->entityManager->flush();

        return $content;
    }

    public function updateExistingContent(Content $content, ContentData $data): void
    {
        $content->fillFromContentData($data);

        if ($data->getUploadResource()) {
            $imageId = $this->imageService->insertImage($content, $data->getUploadResource());
            $content->setImage($imageId);
        }

        $this->entityManager->persist($content);
        $this->entityManager->flush();
    }
}
